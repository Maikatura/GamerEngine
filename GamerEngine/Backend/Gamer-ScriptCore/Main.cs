using System;

public class Main
{

    public float FloatVar { get; set; }

    public Main() 
    {
        Console.WriteLine("Main Constructor!");
    }

    public void PrintMessage() 
    {
        Console.WriteLine("Hellow World from C#!");
    }

    public void PrintCustomMessage(string aMessage)
    {
        Console.WriteLine($"C# says: {aMessage}");
    }
}
