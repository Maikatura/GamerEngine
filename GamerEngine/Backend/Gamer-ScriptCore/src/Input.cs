namespace GamerEngine
{

    public enum MouseLock 
    { 
        None,
        Center,
        CurrentPosition, 
    }

    public class Input
    {
        public static bool IsKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_Keyboard_IsKeyDown(keycode);
        }

        public static bool IsKeyPressed(KeyCode keycode)
        {
            return InternalCalls.Input_Keyboard_IsKeyPressed(keycode);
        }



        public static bool IsMouseDown(KeyCode keycode)
        {
            return InternalCalls.Input_Mouse_IsKeyDown(keycode);
        }


        public static Vector2 GetMouseDelta()
        {
            InternalCalls.Input_Mouse_GetDelta(out Vector2 result);
            return result;
        }

        public static void LockMouse(MouseLock lockMouse)
        {
            InternalCalls.Input_Mouse_LockMouse(lockMouse);
        }
    }
}