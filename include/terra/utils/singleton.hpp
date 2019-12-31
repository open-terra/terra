#pragma once

namespace terra::utils
{
    template<typename T>
    class singleton
    {
    public:
        static T& instance()
        {
            static T inst;
            return inst;
        }

        singleton(singleton const&) = delete;
        singleton& operator=(singleton const&) = delete;

    protected:
        singleton(){};
    };
} // namespace utils