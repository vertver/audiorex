#include "emitters_system.h"
#include <chrono>
#include <thread>

int main() 
{
	std::shared_ptr<arex::game_mixer> sound_mixer = std::make_shared<arex::game_mixer>(arex::get_default_format());
	std::shared_ptr<arex::emitters_callback> sound_callback = std::make_shared<arex::emitters_callback>(sound_mixer);
	std::shared_ptr<arex::audio_hardware> hardware = std::make_shared<arex::audio_hardware>(sound_callback);
	std::shared_ptr<arex::game_source> sound_source = std::make_shared<arex::game_source>("X:\\s.mp3", arex::get_default_format());
	std::shared_ptr<arex::game_emitter> sound_emitter = std::make_shared<arex::game_emitter>(sound_source);
	auto list = hardware->device_list();
	for (auto elem : list) {
		printf("%i device: %s\n", elem.id(), elem.name().c_str());
	}

	sound_mixer->add_source("base_source", sound_source);
	sound_mixer->add_emitter("base_emitter", sound_emitter);
	sound_emitter->set_status(arex::emitter_status::emitter_looped);
	hardware->initialize_device();

	std::this_thread::sleep_for(std::chrono::milliseconds(10000000000000000));

	return 0;
}