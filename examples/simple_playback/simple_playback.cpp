#include "emitters_system.h"

int main() 
{
	std::shared_ptr<arex::game_mixer> sound_mixer = std::make_shared<arex::game_mixer>(arex::get_default_format());
	std::shared_ptr<arex::emitters_callback> sound_callback = std::make_shared<arex::emitters_callback>(sound_mixer);
	std::shared_ptr<arex::audio_hardware> hardware = std::make_shared<arex::audio_hardware>(sound_callback);
	auto list = hardware->device_list();
	for (auto elem : list) {
		printf("%i device: %s\n", elem.id(), elem.name().c_str());
	}

	return 0;
}