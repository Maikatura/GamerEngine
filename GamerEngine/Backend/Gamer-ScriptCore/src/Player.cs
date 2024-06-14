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

        MouseLock myMouseLock = MouseLock.None;
        bool myLeftMouseButtinIsDown = true;

        void OnCreate()
        {
            myTransformComponent = GetComponent<TransformComponent>();
            Console.WriteLine("Test");
        }

        void OnUpdate(float ts)
        {
            //Console.WriteLine($"Player.OnUpdate - {ts}");


            //Console.WriteLine(myTransformComponent.GetChildCount());


            float movementZ = 0.0f;
            float movementX = 0.0f;
            Vector3 moveRotation = new Vector3();


            float rotationSpeed = 100.0f;
            float moveSpeed = 100.0f;


            myLeftMouseButtinIsDown = Input.IsMouseDown(MouseKeyCode.RightButton);
            Input.LockMouse(myLeftMouseButtinIsDown ? MouseLock.Locked : MouseLock.None);

            bool forwardKey = Input.IsKeyDown(KeyCode.W);
            bool backwardKey = Input.IsKeyDown(KeyCode.S);
            bool rightKey = Input.IsKeyDown(KeyCode.D);
            bool leftKey = Input.IsKeyDown(KeyCode.A);

            bool eKey = Input.IsKeyDown(KeyCode.E);
            bool qKey = Input.IsKeyDown(KeyCode.Q);


            if (forwardKey)
            {
                movementZ += ts * moveSpeed;
            }

            if (backwardKey)
            {
                movementZ -= ts * (moveSpeed * 0.5f);
            }

            if (forwardKey && backwardKey)
            {
                movementZ = 0.0f;
            }

            if (myLeftMouseButtinIsDown)
            {
                if (rightKey || eKey)
                {
                    movementX += ts * moveSpeed;
                }

                if (leftKey || qKey)
                {
                    movementX -= ts * moveSpeed;
                }

                if (rightKey && leftKey)
                {
                    movementX = 0.0f;
                }

                Vector2 mouseDelta = Input.GetMouseDelta();
                moveRotation.Y -= mouseDelta.X * ts * rotationSpeed;
               
            }
            else
            {
                if (rightKey)
                {
                    moveRotation.Y -= ts * rotationSpeed;
                }

                if (leftKey)
                {
                    moveRotation.Y += ts * rotationSpeed;
                }

                if (rightKey && leftKey)
                {
                    moveRotation.Y = 0.0f;
                }

                if (eKey)
                {
                    movementX += ts * moveSpeed;
                }

                if (qKey)
                {
                    movementX -= ts * moveSpeed;
                }

                if (eKey && qKey)
                {
                    movementX = 0.0f;
                }
            }

            //Console.WriteLine($"Player.Movement - X:{myTransformComponent.Translation.X} Y:{myTransformComponent.Translation.Y} z:{myTransformComponent.Translation.Z}");
            //Console.WriteLine($"Player.Movement TEST - X:{myTransformComponent.GetForward().X} Y:{myTransformComponent.GetForward().Y} z:{myTransformComponent.GetForward().Z}");
            myTransformComponent.Translation += myTransformComponent.GetForward() * movementZ + myTransformComponent.GetRight() * movementX;
            myTransformComponent.Rotation += moveRotation;


        }

        
    }
}
