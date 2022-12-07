using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats.Bmp;
using SixLabors.ImageSharp.Formats.Tga;
using SixLabors.ImageSharp.PixelFormats;

namespace WaffleBmp
{
    internal static class BMP
    {
        public static void Extract(string outputPath, string bmpPath, string alpPath)
        {
            using var bmpImage = Image.Load(bmpPath);

            if (bmpImage.PixelType.BitsPerPixel != 24 && bmpImage.PixelType.BitsPerPixel != 32)
            {
                throw new Exception("Only 24-bit or 32-bit images are supported.");
            }

            var pixels = new byte[bmpImage.Width * bmpImage.Height * 4];

            using (var image = bmpImage.CloneAs<Rgba32>())
            {
                image.CopyPixelDataTo(pixels);
            }

            var alpImage = File.ReadAllBytes(alpPath);

            var alignedWidth = bmpImage.Width;

            for (var y = 0; y < bmpImage.Height; y++)
            {
                for (var x = 0; x < bmpImage.Width; x++)
                {
                    var src = y * alignedWidth + x;
                    var dst = (y * bmpImage.Width + x) * 4;

                    pixels[dst + 3] = alpImage[src];
                }
            }

            using var finalImage = Image.LoadPixelData<Rgba32>(pixels, bmpImage.Width, bmpImage.Height);

            if (Path.GetExtension(outputPath) == ".tga")
            {
                var encoder = new TgaEncoder
                {
                    BitsPerPixel = TgaBitsPerPixel.Pixel32,
                    Compression = TgaCompression.None
                };

                finalImage.SaveAsTga(outputPath, encoder);
            }
            else
            {
                finalImage.Save(outputPath);
            }
        }

        public static void Create(string srcPath, string bmpPath, string alpPath)
        {
            using var srcImage = Image.Load(srcPath);

            if (srcImage.PixelType.BitsPerPixel != 24 && srcImage.PixelType.BitsPerPixel != 32)
            {
                throw new Exception("Only 24-bit or 32-bit images are supported.");
            }

            using (var image = srcImage.CloneAs<Rgb24>())
            {
                var encoder = new BmpEncoder
                {
                    BitsPerPixel = BmpBitsPerPixel.Pixel24,
                    SupportTransparency = false
                };

                image.SaveAsBmp(bmpPath, encoder);
            }

            using (var image = srcImage.CloneAs<Rgba32>())
            {
                var alignedWidth = srcImage.Width;

                var alpImage = new byte[alignedWidth * srcImage.Height];

                for (var y = 0; y < srcImage.Height; y++)
                {
                    for (var x = 0; x < srcImage.Width; x++)
                    {
                        alpImage[y * alignedWidth + x] = image[x, y].A;
                    }
                }

                File.WriteAllBytes(alpPath, alpImage);
            }
        }
    }
}
