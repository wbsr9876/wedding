package com.example.tomydear;

public class NetCore {
	static {
		System.loadLibrary("RakNet");
	}
	public native int Init();
	public native int Connect(String ip,char port);
	public native int Stop();
	public native int Send();
	public native int Recv();
}
