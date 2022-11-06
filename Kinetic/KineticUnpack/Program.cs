using System.Diagnostics;
using System.Text;

namespace KineticUnpack
{
    internal class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Console.WriteLine("Usage:");
                Console.WriteLine(" KineticUnpack PckPath OutputDirPath");
                Console.ReadKey();
                return;
            }

            Unpack(args[0], args[1]);
        }

        static void Unpack(string filePath, string outputPath)
        {
            using var archiveStream = File.OpenRead(filePath);
            using var archiveReader = new BinaryReader(archiveStream);

            /* Read header */

            var major = archiveReader.ReadInt16();
            var minor = archiveReader.ReadInt16();

            if (major != 1 || minor != 0)
            {
                throw new Exception("Invalid PCK file.");
            }

            var entryCount = archiveReader.ReadInt32();
            var subHeaderSize = archiveReader.ReadInt32();
            var dataIndexPos = archiveReader.ReadInt32();

            archiveReader.ReadInt32(); // zero
            archiveReader.ReadInt32(); // zero
            archiveReader.ReadInt32(); // zero
            archiveReader.ReadInt32(); // zero

            /* Read sub header */

            var subHeaderData = archiveReader.ReadBytes(subHeaderSize);
            // DecryptSubHeader(subHeaderData);
            var subHeaderStream = new MemoryStream(subHeaderData);
            var subHeaderReader = new BinaryReader(subHeaderStream);

            /* Read entry name length */

            var nameLengthList = new List<int>(entryCount);

            for (var i = 0; i < entryCount; i++)
            {
                nameLengthList.Add(subHeaderReader.ReadInt32());
            }

            /* Read entry name */

            var nameList = new List<string>(entryCount);

            for (var i = 0; i < entryCount; i++)
            {
                var bytes = subHeaderReader.ReadBytes(nameLengthList[i]);
                var name = Encoding.Unicode.GetString(bytes);

                nameList.Add(name);
            }

            /* Read data index */

            subHeaderStream.Position = dataIndexPos;

            var entryList = new List<Entry>(entryCount);

            for (var i = 0; i < entryCount; i++)
            {
                entryList.Add(new Entry
                {
                    Offset = subHeaderReader.ReadInt64(),
                    Length = subHeaderReader.ReadInt64(),
                });
            }

            /* Extract all the entries */

            for (var i = 0; i < entryCount; i++)
            {
                var entryName = nameList[i];

                Console.WriteLine($"Extract {entryName}");

                var offset = entryList[i].Offset;
                var length = entryList[i].Length;

                /* Read entry from package */

                archiveStream.Position = offset;
                var data = archiveReader.ReadBytes(Convert.ToInt32(length));

                /* Write entry to disk */

                try
                {
                    var outputFilePath = Path.Combine(outputPath, entryName);
                    var outputFileDir = Path.GetDirectoryName(outputFilePath);

                    if (!string.IsNullOrEmpty(outputFileDir))
                        Directory.CreateDirectory(outputFileDir);

                    File.WriteAllBytes(outputFilePath, data);
                }
                catch (Exception e)
                {
                    Console.WriteLine(e.Message);
                }
            }
        }
    }

    internal class Entry
    {
        public long Offset;
        public long Length;
    }
}