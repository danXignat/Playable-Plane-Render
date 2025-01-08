#include "Sounds.h"

Sounds::Sounds()
{
    //std::cerr << "Checking OpenAL implementation" << std::endl;
    device = alcOpenDevice(nullptr);
    if (!device) {
        std::cerr << "Failed to open OpenAL device." << std::endl;
        return;
    }

    ALCcontext* context = alcCreateContext(device, nullptr);
    if (!alcMakeContextCurrent(context)) {
        //std::cerr << "Failed to set OpenAL context" << std::endl;
        alcCloseDevice(device);
        return;
    }

    std::cout << "OpenAL initialized successfully!" << std::endl;

    // Clean up
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}
