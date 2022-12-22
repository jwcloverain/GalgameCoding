namespace MicoTool
{
    static class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 3)
            {
                Console.WriteLine("Usage:");
                Console.WriteLine(" Create MG2 image : MicoTool -c input.png output.mg2");
                Console.WriteLine();
                Console.WriteLine("Press any key to continue...");
                Console.ReadKey();
                return;
            }

            switch (args[0])
            {
                case "-c":
                {
                    Mico.Create(args[1], args[2]);
                    break;
                }
            }
        }
    }
}