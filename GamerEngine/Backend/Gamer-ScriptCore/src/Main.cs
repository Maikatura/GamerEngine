using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


public class Main
{

    public float FloatVar { get; set; }

    public Main()
    {

        Console.WriteLine("Main Counstructor!");
    }


    public void PrintMessage()
    {
        Console.WriteLine("Hellow World from C#!");
    }

    public void PrintInt(int aMessage)
    {
        Console.WriteLine($"C# Says: {aMessage}");
    }

    public void PrintCustomMessage(string aMessage)
    {
        Console.WriteLine($"C# Says: {aMessage}");
    }
}
