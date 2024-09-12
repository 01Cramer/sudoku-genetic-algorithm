#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class LoadByImage {
public:
	LoadByImage(float width, float height);
	~LoadByImage();

	void draw(sf::RenderWindow& window);

	// Navigating functions
	void move_up();
	void move_down();
	int get_pressed_item();
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
	int selected_item_index;

	sf::Font font;
	sf::Text load_by_image[2];
	float m_width;
	float m_height;
};
