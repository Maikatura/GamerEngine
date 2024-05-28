using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GamerEngine
{
    public class Goober : Entity
    {

        private TransformComponent myTransformComponent;

        private Vector3 myStartPos;

        private float amplitude = 5.0f; // The maximum distance from the central position
        private float frequency = 10.0f; // How fast it oscillates
        private float centralPositionY = 0.0f; // Central position of the object
        private float elapsedTime = 0.0f; // Tracks the elapsed time

        void OnCreate()
        {
            myTransformComponent = GetComponent<TransformComponent>();
            //Console.WriteLine("Test");

            myStartPos = myTransformComponent.Translation;
        }

        void OnUpdate(float ts)
        {
            elapsedTime += ts;
            float newYPosition = myStartPos.Y + (float)Math.Sin(elapsedTime * frequency) * amplitude;

            //Console.WriteLine($"{elapsedTime}");

            // Apply newYPosition to your object here
            // For example, if this is a Unity script, you might use:
            myTransformComponent.Translation = new Vector3(myTransformComponent.Translation.X, newYPosition, myTransformComponent.Translation.Z);
        }

        

        

     
    }
}
