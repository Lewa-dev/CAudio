#pragma once
#include "OpenAL.h"
#include <assert.h>
#include <string>
#include <iostream>

namespace Debug
{

    static std::string getErrorText(ALenum error)
    {
        if (error == AL_INVALID_NAME)
        {
            return "Invalid name";
        }
        else if (error == AL_INVALID_ENUM)
        {
            return " Invalid enum ";
        }
        else if (error == AL_INVALID_VALUE)
        {
            return " Invalid value ";
        }
        else if (error == AL_INVALID_OPERATION)
        {
            return " Invalid operation ";
        }
        else if (error == AL_OUT_OF_MEMORY)
        {
            return " Out of memory like! ";
        }

        return " Don't know ";
    }

    static void AlerrorAssert()
    {
        ALenum error = alGetError();
        if (error != AL_NO_ERROR)
        {
            std::cout << "ALerror: " << getErrorText(error) << std::endl;
        }
        assert(error == AL_NO_ERROR);
    }
}
