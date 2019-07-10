#include <SFML/Graphics.hpp>
#include <iostream>

void drawBlur(sf::RenderWindow& window, int blurRadius = 1, int blurCount = 1, bool outputText = false, bool betterBlur = false);
void screenshot(sf::RenderWindow& window);

int main() {

	sf::RenderWindow window(sf::VideoMode(1280, 720), "Blur Effects");

	bool windowIsOpen = true;
	bool drawn = false;

	while (windowIsOpen) {

		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window.close();
				windowIsOpen = false;
			}
		}

		if (!drawn) {

			window.clear();

			sf::Texture image;
			image.loadFromFile("Resources//TestImage.jpg");

			sf::RectangleShape shape(sf::Vector2f(window.getSize()));
			shape.setTexture(&image);
			window.draw(shape);

			drawBlur(window, 15, 1, false, false);
			screenshot(window);

			window.display();

			drawn = true;

		}

	}

}

void drawBlur(sf::RenderWindow& window, int blurRadius, int blurCount, bool outputText, bool betterBlur) {


	sf::Texture currentScreenT;
	currentScreenT.create(window.getSize().x, window.getSize().y);
	currentScreenT.update(window);

	sf::Image currentScreen = currentScreenT.copyToImage();
	sf::Image blur;
	blur.create(currentScreen.getSize().x, currentScreen.getSize().y, sf::Color::Black);

	for (int i = 0; i < blurCount; i++) {

		if (outputText)
			std::cout << "Pass " << i + 1 << " out of " << blurCount << std::endl;

		for (int x = 0; x < currentScreen.getSize().x; x++) {
			for (int y = 0; y < currentScreen.getSize().y; y++) {

				int totalR = 0;
				int totalG = 0;
				int totalB = 0;

				for (int dx = -blurRadius; dx <= blurRadius; dx++) {
					for (int dy = -blurRadius; dy <= blurRadius; dy++) {

						int pixelX = x + dx;
						int pixelY = y + dy;

						if (pixelX >= 0 && pixelX < currentScreen.getSize().x &&
							pixelY >= 0 && pixelY < currentScreen.getSize().y) {
							sf::Color curValue = currentScreen.getPixel(pixelX, pixelY);

							if (betterBlur) {
								totalR += curValue.r * curValue.r;
								totalG += curValue.g * curValue.g;
								totalB += curValue.b * curValue.b;
							} else {
								totalR += curValue.r;
								totalG += curValue.g;
								totalB += curValue.b;
							}

						}
					}
				}

				totalR /= (pow(blurRadius * 2 + 1, 2));
				totalG /= (pow(blurRadius * 2 + 1, 2));
				totalB /= (pow(blurRadius * 2 + 1, 2));

				if (betterBlur) {
					totalR = sqrt(totalR);
					totalG = sqrt(totalG);
					totalB = sqrt(totalB);
				}

				blur.setPixel(x, y, sf::Color(totalR, totalG, totalB));

			}
		}

		currentScreen = blur;

	}


	sf::Texture blurT;
	blurT.loadFromImage(blur);

	sf::Sprite blurS;
	blurS.setTexture(blurT, true);
	window.draw(blurS);

}

void screenshot(sf::RenderWindow& window) {
	sf::Texture currentScreenT;
	currentScreenT.create(window.getSize().x, window.getSize().y);
	currentScreenT.update(window);

	sf::Image currentScreen = currentScreenT.copyToImage();
	currentScreen.saveToFile("Resources//Output.png");
}