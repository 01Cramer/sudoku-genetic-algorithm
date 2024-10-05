#pragma once
#include <SFML/Graphics.hpp>
#include "Controller.h"
#include <string>

class LoadByImage : public Controller {
public:
	LoadByImage(float width, float height, uint8_t number_of_items);
	~LoadByImage();

	void draw(sf::RenderWindow& window);

	std::string get_path_from_clipboard();

	void clear_path_to_image();
	void strip_path_to_image();
	std::string path_to_image;
	sf::Text input_path;

	std::string execute_python_script(const std::string& script_path, const std::string& image_path);
	int display_processing_information();
	
	float get_width();
	float get_height();

private:
	sf::Font font;
	float m_width;
	float m_height;
};
