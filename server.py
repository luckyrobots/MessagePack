#!/usr/bin/env python3

import asyncio
import websockets
import msgpack
import json
from datetime import datetime

async def handle_message(websocket):
    print(f"New client connected at {datetime.now()}")
    
    try:
        async for message in websocket:
            try:
                # Decode the MessagePack data, ensuring keys are strings
                decoded = msgpack.unpackb(message, raw=False)
                
                # Convert binary data to hex for display (check for string key 'data')
                if 'data' in decoded and isinstance(decoded['data'], bytes):
                    decoded['data'] = f"<binary data: {len(decoded['data'])} bytes>"
                
                # Pretty print the message
                print("\nReceived Message:")
                print(json.dumps(decoded, indent=2))
                
            except msgpack.ExtraData as e:
                print(f"Error decoding MessagePack data: {e}")
            except Exception as e:
                print(f"Error processing message: {e}")
                
    except websockets.exceptions.ConnectionClosed:
        print(f"Client disconnected at {datetime.now()}")

async def main():
    # Start WebSocket server
    server = await websockets.serve(
        handle_message,
        "localhost",  # Host
        8765,        # Port
        ping_interval=None  # Disable ping to avoid interference with MessagePack
    )
    
    print("WebSocket server started on ws://localhost:3000")
    print("Waiting for connections...")
    
    # Keep the server running
    await server.wait_closed()

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nServer stopped by user") 