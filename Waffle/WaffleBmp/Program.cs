namespace WaffleBmp
{
    internal class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 4)
            {
                Console.WriteLine("Usage:");
                Console.WriteLine("  Combine BMP & ALP to PNG  : WaffleBmp -e in.bmp in.alp out.png");
                Console.WriteLine("  Combine BMP & ALP to TGA  : WaffleBmp -e in.bmp in.alp out.tga");
                Console.WriteLine("  Create BMP & ALP from PNG : WaffleBmp -c in.png out.bmp out.alp");
                return;
            }

            switch (args[0])
            {
                case "-e":
                {
                    // out bmp alp
                    BMP.Extract(args[3], args[1], args[2]);
                    break;
                }
                case "-c":
                {
                    // src bmp alp
                    BMP.Create(args[1], args[2], args[3]);
                    break;
                }
            }
        }
    }
}