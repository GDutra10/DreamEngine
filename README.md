# DreamEngine

The main idea of this repository is to develop a Game Engine from scratch, just for learning purposes.
The Game Engine is developed with C++, using OpenGL for the graphics and C# for the Script Engine using coreclr.

The project is Divided by:
- Core
- CSharpCore
- Editor

## Core
Everything about what an Engine must-have, like ECS, GameSystem, Inputs, IO, Loggers, Render, Resources, and Scripting.

https://github.com/user-attachments/assets/f6863026-93e7-4dc2-bcf7-912a6efb51dd

## CSharpCore
It's C# API that helps to create scripts for the game. The idea is to facilitate the game development and make it easy to edit a script, compile, and run everything without recompiling the Game Engine and also use the power of the C# programing language. A little bit inspired by Unity.

just an example of a script: 
```csharp

using DreamEngine.Inputs;
using DreamEngine.Scripting;

namespace MyGame;

public class PlayerScript : Script
{
	private float _velocity = 0;

	public override void Start()
	{
		_velocity = 0.01f;
	}

	public override void Update()
	{
		if (Input.IsPressed(Key.D))
			this.Entity.Transform.Position.X += _velocity;
		if (Input.IsPressed(Key.A))
			this.Entity.Transform.Position.X -= _velocity;
		if (Input.IsPressed(Key.W))
			this.Entity.Transform.Position.Y -= _velocity;
		if (Input.IsPressed(Key.S))
			this.Entity.Transform.Position.Y += _velocity;
	}
}
```

## Editor
The Editor developed with the Game Engine (Core folder) and using ImGui.

https://github.com/user-attachments/assets/5041d83b-3c3d-440b-b202-f3dd6ed5770d

Video example of the Editor working with the script system.

## Disclaimer
Notice that this Engine is not capable of building a full game, because it's a project for learning purposes and has a lot of bugs, especially in the Editor.
