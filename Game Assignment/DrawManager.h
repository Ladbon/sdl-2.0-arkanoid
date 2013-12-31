#pragma once

class Entity;

class DrawManager {
	friend class SpriteManager;

public:
	DrawManager();
	DrawManager(struct SDL_Window *window, int w, int h) {
		this->Initialize(window, w, h);
	}

	~DrawManager();

	bool Initialize(struct SDL_Window *window, int _width, int _height);
	void Cleanup();

	void Clear();
	void Present();

	void Draw(Entity *entity, int x, int y);

	int getWidth() { return width; }
	int getHeight() { return height; }
	struct SDL_Renderer* getRenderer() { return renderer; }

private:
	int width;
	int height;
	struct SDL_Renderer *renderer;
};
