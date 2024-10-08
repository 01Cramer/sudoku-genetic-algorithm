#include "LoadByImage.h"
#include <array>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <Windows.h>

LoadByImage::LoadByImage(float width, float height, uint8_t number_of_items) : Controller(number_of_items) {
	if (!font.loadFromFile("Fonts/Alice-Regular.ttf")) {
		throw std::exception("Cant find font!");
	}

	menu[0].setFont(font);
	menu[0].setCharacterSize(64);
	menu[0].setFillColor(sf::Color(111, 0, 255));
	menu[0].setString("Confirm Path Of Image");
	menu[0].setPosition(sf::Vector2f(width / 2, height / 6));
	sf::FloatRect textRect = menu[0].getLocalBounds();
	menu[0].move(-textRect.width / 2.0f, 0);

	menu[1].setFont(font);
	menu[1].setCharacterSize(64);
	menu[1].setFillColor(sf::Color::Black);
	menu[1].setString("Back");
	menu[1].setPosition(sf::Vector2f(width / 2, height /  3));
	textRect = menu[1].getLocalBounds();
	menu[1].move(-textRect.width / 2.0f, 0);

	input_path.setFont(font);
	input_path.setCharacterSize(20);
	input_path.setFillColor(sf::Color::White);
	input_path.setString("Paste Path Here (Ctrl + V): ");
	input_path.setPosition(sf::Vector2f(width / 2, height / 2));
	sf::FloatRect inputRect = input_path.getLocalBounds();
	input_path.move(-inputRect.width / 2.0f, 0);

	selected_item_index = 0;
	path_to_image = "";
	m_width = width;
	m_height = height;
}

LoadByImage :: ~LoadByImage() {}

void LoadByImage::draw(sf::RenderWindow& window) {
	for (int i = 0; i < 2; i++) {
		window.draw(menu[i]);
	}

	window.draw(input_path);
}

void LoadByImage::clear_path_to_image() {
	path_to_image = "";
	input_path.setString("Paste Path Here (Ctrl + V): ");

	sf::FloatRect inputRect = input_path.getLocalBounds();
	input_path.setPosition(sf::Vector2f(m_width / 2, m_height / 2));
	input_path.move(-inputRect.width / 2.0f, 0);
}

std::string LoadByImage::get_path_from_clipboard() {
	if (!OpenClipboard(nullptr))
		return "";

	HANDLE h_data = GetClipboardData(CF_TEXT);
	if (h_data == nullptr) {
		CloseClipboard();
		return "";
	}

	char* h_data_to_char = static_cast<char*>(GlobalLock(h_data));
	if (h_data_to_char == nullptr) {
		CloseClipboard();
		return "";
	}

	std::string path(h_data_to_char);
	GlobalUnlock(h_data);
	CloseClipboard();

	return path;
}

std::string LoadByImage::execute_python_script(const std::string& script_path, const std::string& image_path){
	std::string temp_file_path = "encoded_board.txt";
	std::ofstream temp_file(temp_file_path);
	temp_file.close();

	std::string command = "python " + script_path + " " + image_path;

	std::system(command.c_str());

	std::ifstream temp_file_stream(temp_file_path);

	if (!temp_file_stream) {
		std::cerr << "Can't read from file." << std::endl;
		return "";
	}

	std::string encoded_board((std::istreambuf_iterator<char>(temp_file_stream)),
		std::istreambuf_iterator<char>());

	temp_file_stream.close();

	return encoded_board;
}

void LoadByImage::strip_path_to_image() {
	std::string temp_path = path_to_image;
	path_to_image = "";
	for (int i = 1; i < temp_path.size() - 1; i++) {
		path_to_image += temp_path[i];
	}
}

float LoadByImage::get_width() {
	return m_width;
}
float LoadByImage::get_height() {
	return m_height;
}

int LoadByImage::display_processing_information() {
	int msgboxID = MessageBox(
		NULL,
		L"We will process your image\nPlease press OK.",
		L"Processing Image",
		MB_ICONINFORMATION | MB_OK
	);

	return msgboxID;
}
