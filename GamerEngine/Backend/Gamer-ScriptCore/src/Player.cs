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
        private TransformComponent myChildTransform;

        MouseLock myMouseLock = MouseLock.None;
        bool myRightMouseButtinIsDown = false;
        bool myLeftMouseButtinIsDown = false;

        float myMinZoom = 10.0f;
        float myMaxZoom = 20.0f;



        void OnCreate()
        {
            myTransformComponent = GetComponent<TransformComponent>();
            myChildTransform = myTransformComponent.GetChild(0).GetComponent<TransformComponent>();
        }

        void OnUpdate(float ts)
        {
            //Console.WriteLine($"Player.OnUpdate - {ts}");


            //Console.WriteLine(myTransformComponent.GetChildCount());


            myChildTransform.GetChild(0).Translation +=  myChildTransform.GetChild(0).Transform.GetForward() * Input.GetScrollWheel() * 10.0f;




            float movementZ = 0.0f;
            float movementX = 0.0f;
            Vector3 moveRotation = new Vector3();


            float rotationSpeed = 100.0f;
            float moveSpeed = 100.0f;


            myRightMouseButtinIsDown = Input.IsMouseDown(MouseKeyCode.RightButton);
            myLeftMouseButtinIsDown = Input.IsMouseDown(MouseKeyCode.LeftButton);
            bool myLeftMouseButtinIsDownThisFrame = Input.IsMousePressedThisFrame(MouseKeyCode.RightButton);
            Input.LockMouse(myRightMouseButtinIsDown ? MouseLock.Locked : MouseLock.None);

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

            if (myLeftMouseButtinIsDownThisFrame)
            {
                moveRotation = myChildTransform.Rotation;
            }

            if (myRightMouseButtinIsDown)
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
                moveRotation.Y += mouseDelta.X * ts * rotationSpeed;
               
            }
            else if (myLeftMouseButtinIsDown)
            {
                moveRotation += new Vector3(0.0f, 1.0f, 0.0f) * Input.GetMouseDelta().X;

                if (rightKey)
                {
                    moveRotation.Y += ts * rotationSpeed;
                }

                if (leftKey)
                {
                    moveRotation.Y -= ts * rotationSpeed;
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
            
            if (myRightMouseButtinIsDown)
            {
                myChildTransform.Rotation = new Vector3(0.0f, 0.0f, 0.0f);
                myTransformComponent.Rotation += moveRotation;
            }
            else if(myLeftMouseButtinIsDown)
            {
                myChildTransform.Rotation += moveRotation;
            }
        }
    }
}
