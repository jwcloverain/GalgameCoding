using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;

namespace MicoTool
{
    class Mico
    {
        public static void Create(string sourcePath, string outputPath)
        {
            var source = Image.Load(sourcePath);

            if (source.PixelType.BitsPerPixel != 24 && source.PixelType.BitsPerPixel != 32)
            {
                throw new Exception("Only 24-bit and 32-bit image are supported.");
            }

            // Stage 1 : Flip
            source.Mutate(x => x.Flip(FlipMode.Vertical));

            // Stage 2 : Generate color image of source.
            var colorLayer = source.CloneAs<Rgb24>();
            byte[] colorLayerData;
            using (var colorLayerStream = new MemoryStream())
            {
                colorLayer.SaveAsPng(colorLayerStream);
                colorLayerData = colorLayerStream.ToArray();
            }
            
            // Stage 3 : Generate alpha image of source.
            var alphaLayerData = Array.Empty<byte>();
            if (source.PixelType.BitsPerPixel == 32)
            {
                var alphaLayer = source.CloneAs<Rgba32>();
                var alphaLayerPixels = new byte[source.Width * source.Height * 3];
                
                for (var y = 0; y < source.Height; y++)
                {
                    for (var x = 0; x < source.Width; x++)
                    {
                        var j = (y * source.Width + x) * 3;

                        alphaLayerPixels[j + 0] = alphaLayer[x, y].A;
                        alphaLayerPixels[j + 1] = alphaLayer[x, y].A;
                        alphaLayerPixels[j + 2] = alphaLayer[x, y].A;
                    }
                }

                var alphaLayerImage = Image.LoadPixelData<Rgb24>(alphaLayerPixels, source.Width, source.Height);

                using (var alphaLayerStream = new MemoryStream())
                {
                    alphaLayerImage.SaveAsPng(alphaLayerStream);
                    alphaLayerData = alphaLayerStream.ToArray();
                }
            }
            
            // Stage 4 : Write
            using (var stream = File.Create(outputPath))
            using (var writer = new BinaryWriter(stream))
            {
                writer.Write(0x4F43494D); // "MICO"
                writer.Write(0x31304743); // "CG01"
                writer.Write(colorLayerData.Length);
                writer.Write(alphaLayerData.Length);

                if (colorLayerData.Length > 0)
                {
                    EncryptImageData(colorLayerData);
                    writer.Write(colorLayerData);
                }

                if (alphaLayerData.Length > 0)
                {
                    EncryptImageData(alphaLayerData);
                    writer.Write(alphaLayerData);
                }
                
                writer.Flush();
            }
        }

        private static void EncryptImageData(byte[] data)
        {
            var threshold = Math.Min(25, data.Length);
            var key = (byte)data.Length;

            for (var i = 0; i < threshold; i++)
            {
                data[i] ^= (byte)(key + i);
            }
        }
    }
}