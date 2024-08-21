using UnityEngine;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Collections.Generic;

public class BoundingBoxReceiver : MonoBehaviour
{
    [StructLayout(LayoutKind.Sequential)]
    public struct BoundingBox
    {
        public int x;
        public int y;
        public int width;
        public int height;
    }

    TcpListener listener;
    List<GameObject> objects = new List<GameObject>();

    void Start()
    {
        listener = new TcpListener(System.Net.IPAddress.Parse("127.0.0.1"), 65432);
        listener.Start();
    }

    void Update()
    {
        if (listener.Pending())
        {
            using (TcpClient client = listener.AcceptTcpClient())
            {
                NetworkStream stream = client.GetStream();
                BoundingBox box;

                while (stream.DataAvailable)
                {
                    byte[] buffer = new byte[Marshal.SizeOf(typeof(BoundingBox))];
                    stream.Read(buffer, 0, buffer.Length);

                    box = ByteArrayToStructure<BoundingBox>(buffer);

                    GameObject obj = GameObject.CreatePrimitive(PrimitiveType.Cube);
                    obj.transform.position = new Vector3(box.x / 100.0f, box.y / 100.0f, 0);
                    obj.transform.localScale = new Vector3(box.width / 100.0f, box.height / 100.0f, 1);
                    objects.Add(obj);
                }
            }
        }
    }

    private T ByteArrayToStructure<T>(byte[] bytes)
    {
        GCHandle handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
        T stuff = (T)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(T));
        handle.Free();
        return stuff;
    }
}
