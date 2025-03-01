﻿using GamerEngine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GamerEngine
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
            }
        }

        public Vector3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
            }
        }

        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
            }
        }

        public Vector3 GetForward()
        {
            InternalCalls.TransformComponent_GetForward(Entity.ID, out Vector3 translation);
            return translation;
        }

        public Vector3 GetRight()
        {
            InternalCalls.TransformComponent_GetRight(Entity.ID, out Vector3 translation);
            return translation;
        }

        public Vector3 GetUp()
        {
            InternalCalls.TransformComponent_GetUp(Entity.ID, out Vector3 translation);
            return translation;
        }

        public bool HasChildren()
        {
            return InternalCalls.Entity_HasChildren(Entity.ID);
        }

        public int GetChildCount()
        {
            return InternalCalls.Entity_GetChildCount(Entity.ID);
        }

        public Entity GetChild(int index)
        {
            ulong childID = InternalCalls.Entity_GetChild(Entity.ID, index);
            return new Entity(childID);
        }

    }

}