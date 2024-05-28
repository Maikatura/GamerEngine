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

         

            float movement = 0.0f;
            Vector3 moveRotation = new Vector3();


            float rotationSpeed = 100.0f;
            float moveSpeed = 100.0f;

            if (Input.IsKeyDown(KeyCode.W))
            {
                movement += ts * moveSpeed;
               
   
            }

            if (Input.IsKeyDown(KeyCode.S))
            {
                movement -= ts * moveSpeed;
            }

            if (Input.IsKeyDown(KeyCode.D))
            {
                moveRotation.Y += ts * rotationSpeed;
            }

            if (Input.IsKeyDown(KeyCode.A))
            {
                moveRotation.Y -= ts * rotationSpeed;
            }


            //Console.WriteLine($"Player.Movement - X:{myTransformComponent.Translation.X} Y:{myTransformComponent.Translation.Y} z:{myTransformComponent.Translation.Z}");
            //Console.WriteLine($"Player.Movement TEST - X:{myTransformComponent.GetForward().X} Y:{myTransformComponent.GetForward().Y} z:{myTransformComponent.GetForward().Z}");
            myTransformComponent.Translation += myTransformComponent.GetForward() * movement;
            myTransformComponent.Rotation += moveRotation;



        }

        
    }
}
