using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace TextTool
{
    internal class TextMapFile
    {
        readonly SortedDictionary<uint, string> _stringMap = new();

        public void Load(string ptrFilePath, string txdFilePath)
        {
            using var ptrStream = File.OpenRead(ptrFilePath);
            using var ptrReader = new BinaryReader(ptrStream);

            using var txdStream = File.OpenRead(txdFilePath);
            using var txdReader = new BinaryReader(txdStream);

            if (ptrReader.ReadInt32() != 0x20525450)
            {
                throw new Exception("Invalid PTR file.");
            }

            var count = ptrReader.ReadInt32();

            if (16 + count * 12 != ptrStream.Length)
            {
                throw new Exception("Invalid PTR file.");
            }

            ptrReader.ReadInt32(); // zero
            ptrReader.ReadInt32(); // zero

            _stringMap.Clear();

            for (var i = 0; i < count; i++)
            {
                var key = ptrReader.ReadUInt32();
                var offset = (long)ptrReader.ReadInt32();
                var length = ptrReader.ReadInt32();

                if (offset < 0 || offset >= txdStream.Length || offset + length > txdStream.Length)
                {
                    throw new Exception("Broken PTR file.");
                }

                txdStream.Position = offset;
                var bytes = txdReader.ReadBytes(length);
                var str = Encoding.UTF8.GetString(bytes);

                if (!_stringMap.TryAdd(key, str))
                {
                    Console.WriteLine("WARNING: Duplicate string key.");
                }
            }
        }

        public void Save(string ptrFilePath, string txdFilePath)
        {
            using var ptrStream = File.Create(ptrFilePath);
            using var ptrWriter = new BinaryWriter(ptrStream);

            using var txdStream = File.Create(txdFilePath);
            using var txdWriter = new BinaryWriter(txdStream);

            ptrWriter.Write(0x20525450);
            ptrWriter.Write(_stringMap.Count);
            ptrWriter.Write(0);
            ptrWriter.Write(0);

            foreach (var e in _stringMap)
            {
                var bytes = Encoding.UTF8.GetBytes(e.Value);

                ptrWriter.Write(e.Key);
                ptrWriter.Write(Convert.ToUInt32(txdStream.Position));
                ptrWriter.Write(bytes.Length);

                txdWriter.Write(bytes);
            }

            ptrWriter.Flush();
            txdWriter.Flush();
        }

        public void ExportText(string filePath)
        {
            using var writer = File.CreateText(filePath);

            foreach (var e in _stringMap)
            {
                var s = e.Value;

                writer.WriteLine($"◇{e.Key:X8}◇{s}");
                writer.WriteLine($"◆{e.Key:X8}◆{s}");
                writer.WriteLine();
            }

            writer.Flush();
        }

        public void ImportText(string filePath, bool merge)
        {
            if (!merge)
            {
                _stringMap.Clear();
            }

            using var reader = File.OpenText(filePath);

            var lineNo = 0;

            while (!reader.EndOfStream)
            {
                var ln = lineNo;
                var line = reader.ReadLine();
                lineNo++;

                if (line == null)
                    break;

                if (line.Length == 0 || line[0] != '◆')
                    continue;

                var m = Regex.Match(line, @"◆(\w+)◆(.+$)");

                if (!m.Success || m.Groups.Count != 3)
                    throw new Exception($"Bad format at line: {ln}");

                var key = uint.Parse(m.Groups[1].Value, NumberStyles.HexNumber);
                var str = m.Groups[2].Value;

                if (merge)
                {
                    if (!_stringMap.ContainsKey(key))
                    {
                        throw new Exception($"Invalid key at line: {ln}");
                    }

                    _stringMap[key] = str;
                }
                else
                {
                    _stringMap.Add(key, str);
                }
            }
        }
    }
}
