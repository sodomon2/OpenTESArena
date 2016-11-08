#include <cassert>

#include "SDL.h"

#include "LogbookPanel.h"

#include "Button.h"
#include "GameWorldPanel.h"
#include "TextAlignment.h"
#include "TextBox.h"
#include "../Game/GameState.h"
#include "../Math/Int2.h"
#include "../Media/Color.h"
#include "../Media/FontManager.h"
#include "../Media/FontName.h"
#include "../Media/PaletteFile.h"
#include "../Media/PaletteName.h"
#include "../Media/TextureFile.h"
#include "../Media/TextureManager.h"
#include "../Media/TextureName.h"
#include "../Rendering/Renderer.h"
#include "../Rendering/Texture.h"

LogbookPanel::LogbookPanel(GameState *gameState)
	: Panel(gameState)
{
	this->titleTextBox = [gameState]()
	{
		Int2 center(Renderer::ORIGINAL_WIDTH / 2, Renderer::ORIGINAL_HEIGHT / 2);
		Color color(255, 207, 12);
		std::string text = "Your logbook is empty.";
		auto &font = gameState->getFontManager().getFont(FontName::A);
		auto alignment = TextAlignment::Center;
		return std::unique_ptr<TextBox>(new TextBox(
			center,
			color,
			text,
			font,
			alignment,
			gameState->getRenderer()));
	}();

	this->backButton = []()
	{
		Int2 center(Renderer::ORIGINAL_WIDTH - 40, Renderer::ORIGINAL_HEIGHT - 13);
		auto function = [](GameState *gameState)
		{
			std::unique_ptr<Panel> backPanel(new GameWorldPanel(gameState));
			gameState->setPanel(std::move(backPanel));
		};
		return std::unique_ptr<Button>(new Button(center, 34, 14, function));
	}();
}

LogbookPanel::~LogbookPanel()
{

}

void LogbookPanel::handleEvents(bool &running)
{
	auto mousePosition = this->getMousePosition();
	auto mouseOriginalPoint = this->getGameState()->getRenderer()
		.nativePointToOriginal(mousePosition);

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		bool applicationExit = (e.type == SDL_QUIT);
		bool resized = (e.type == SDL_WINDOWEVENT) &&
			(e.window.event == SDL_WINDOWEVENT_RESIZED);
		bool escapePressed = (e.type == SDL_KEYDOWN) &&
			(e.key.keysym.sym == SDLK_ESCAPE);
		bool lPressed = (e.type == SDL_KEYDOWN) &&
			(e.key.keysym.sym == SDLK_l);

		if (applicationExit)
		{
			running = false;
		}
		if (resized)
		{
			int width = e.window.data1;
			int height = e.window.data2;
			this->getGameState()->resizeWindow(width, height);
		}
		if (escapePressed || lPressed)
		{
			this->backButton->click(this->getGameState());
		}

		bool leftClick = (e.type == SDL_MOUSEBUTTONDOWN) &&
			(e.button.button == SDL_BUTTON_LEFT);		

		if (leftClick && this->backButton->contains(mouseOriginalPoint))
		{
			this->backButton->click(this->getGameState());
		}
	}
}

void LogbookPanel::handleMouse(double dt)
{
	static_cast<void>(dt);
}

void LogbookPanel::handleKeyboard(double dt)
{
	static_cast<void>(dt);
}

void LogbookPanel::tick(double dt, bool &running)
{
	static_cast<void>(dt);

	this->handleEvents(running);
}

void LogbookPanel::render(Renderer &renderer)
{
	// Clear full screen.
	renderer.clearNative();
	renderer.clearOriginal();

	// Set palette.
	auto &textureManager = this->getGameState()->getTextureManager();
	textureManager.setPalette(PaletteFile::fromName(PaletteName::Default));

	// Draw logbook background.
	const auto &logbookBackground = textureManager.getTexture(
		TextureFile::fromName(TextureName::Logbook), 
		PaletteFile::fromName(PaletteName::BuiltIn));
	renderer.drawToOriginal(logbookBackground.get());

	// Draw text: title.
	renderer.drawToOriginal(this->titleTextBox->getTexture(),
		this->titleTextBox->getX(), this->titleTextBox->getY());

	// Scale the original frame buffer onto the native one.
	renderer.drawOriginalToNative();

	// Draw cursor.
	const auto &cursor = textureManager.getTexture(
		TextureFile::fromName(TextureName::SwordCursor));
	auto mousePosition = this->getMousePosition();
	renderer.drawToNative(cursor.get(),
		mousePosition.getX(), mousePosition.getY(),
		static_cast<int>(cursor.getWidth() * this->getCursorScale()),
		static_cast<int>(cursor.getHeight() * this->getCursorScale()));
}
