#include "emitters_system.h"
#include <chrono>
#include <thread>

#define SOUND_TO_PLAY "X:\\s.mp3"
#define SOUND_TO_PLAY_SECOND "I:\\Downloads\\Output.mp3"

int main() 
{
	std::shared_ptr<arex::game_mixer> sound_mixer = std::make_shared<arex::game_mixer>(arex::get_default_format());
	std::shared_ptr<arex::emitters_callback> sound_callback = std::make_shared<arex::emitters_callback>(sound_mixer);
	std::shared_ptr<arex::audio_hardware> hardware = std::make_shared<arex::audio_hardware>(sound_callback);
	auto list = hardware->device_list();
	for (auto elem : list) {
		printf("%i device: %s\n", elem.id(), elem.name().c_str());
	}

// 	std::shared_ptr<arex::game_source> sound_source = std::make_shared<arex::game_source>(SOUND_TO_PLAY, arex::get_default_format());
// 	std::shared_ptr<arex::game_emitter> sound_emitter = std::make_shared<arex::game_emitter>(sound_source);
// 	sound_mixer->add_source("base_source", sound_source);
// 	sound_mixer->add_emitter("base_emitter", sound_emitter);

	std::shared_ptr<arex::game_source> sound_source_second = std::make_shared<arex::game_source>(SOUND_TO_PLAY_SECOND, arex::get_default_format());
	std::shared_ptr<arex::game_emitter> sound_emitter_second = std::make_shared<arex::game_emitter>(sound_source_second);
	sound_mixer->add_source("base_source2", sound_source_second);
	sound_mixer->add_emitter("base_emitter2", sound_emitter_second);

	//sound_emitter->set_status(arex::emitter_status::emitter_looped);
	sound_emitter_second->set_status(arex::emitter_status::emitter_looped);

	hardware->initialize_device();

	getchar();
	return 0;
}