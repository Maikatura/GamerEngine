using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GamerEngine
{
    public class Player : Entity
    {
        private TransformComponent myTransformComponent;


        void OnCreate()
        {
            myTransformComponent = GetComponent<TransformComponent>();
            Console.WriteLine("Test");
        }

        void OnUpdate(float ts)
        {
            //Console.WriteLine($"Player.OnUpdate - {ts}");

            if (HasComponent<TransformComponent>())
            {
                Console.WriteLine("Has it!");
            }


            Vector3 movement = GetComponent<TransformComponent>().Translation;
            Console.WriteLine($"Player.Movement - X:{movement.X} Y:{movement.Y} z:{movement.Z}");


            if (Input.IsKeyDown(KeyCode.W))
            {
                movement.Z += ts;
                Console.WriteLine("Pressing");

               
            }

            GetComponent<TransformComponent>().Translation += movement;
        }

        
    }
}
