namespace TextTool
{
    internal class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 4)
            {
                Console.WriteLine("Usage:");
                Console.WriteLine("  Extract to text file  : TextTool -e input.ptr input.txd output.txt");
                Console.WriteLine("  Create from text file : TextTool -c input.txt output.ptr output.txd");
                Console.WriteLine("Press any key to continue...");
                Console.ReadKey();
                return;
            }

            switch (args[0])
            {
                case "-e":
                {
                    var file = new TextMapFile();
                    file.Load(args[1], args[2]);
                    file.ExportText(args[3]);
                    break;
                }
                case "-c":
                {
                    var file = new TextMapFile();
                    file.ImportText(args[1], false);
                    file.Save(args[2], args[3]);
                    break;
                }
            }
        }
    }
}