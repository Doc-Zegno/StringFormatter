#pragma once

#include <string>
#include <sstream>
#include <cstring>
#include <stdexcept>
#include <type_traits>


namespace Handmada {
    /// <summary>
    /// Provides functionality for building formatted strings
    /// </summary>
    class StringFormatter {
    private:
        static void raiseException(const std::string& message)
        {
            throw std::runtime_error(
                std::string("StringFormatter failure: ") + message
            );
        }


        template<typename T>
        static void inject(std::ostream& out, const T& value, const char* options)
        {
            if (*options == '\0') {
                out << value;
            } else {
                if (!strcmp(options, ":x")) {
                    // Hex
                    // Remove `constexpr` below if using C++17 is not an option
                    if constexpr (std::is_integral<T>::value) {
                        out << std::hex;
                        out << value;
                        out << std::dec;
                    } else {
                        raiseException("\":x\" option is not compatible with non-integral type");
                    }
                } else {
                    // Unknown option
                    raiseException(std::string("unrecognized options \"") + options + "\"");
                }
            }
        }


        template<typename T, typename... TArgs>
        static void step(std::ostream& out, char* pattern, const T& value, const TArgs&... args)
        {
            auto p = std::strchr(pattern, '{');
            if (!p) {
                raiseException("placeholder {} wasn't found");
            }

            auto q = std::strchr(p + 1, '}');
            if (!q) {
                raiseException("closing braces } weren't found");
            }

            *p = '\0';
            *q = '\0';
            out << pattern;
            inject(out, value, p + 1);

            step(out, q + 1, args...);
        }


        static void step(std::ostream& out, char* pattern)
        {
            auto p = std::strchr(pattern, '{');
            if (p) {
                raiseException("opening braces { were found but no arguments were provided");
            }
            out << pattern;
        }


        std::string _pattern;

    public:
        /// <param name="pattern">
        ///     Pattern of string to be built
        ///     <para>
        ///         **Example**: pattern "({}, {}, {})" and values 1, 2, 3 will produce
        ///         "(1, 2, 3)"
        ///     </para>
        ///     <para>
        ///         **Note**: use {:x} for hex
        ///     </para>
        /// </param>
        StringFormatter(const std::string& pattern) : _pattern(pattern) { }


        /// <summary>
        /// Build string using stored pattern and input values.
        /// </summary>
        /// <param name="args">Values to be injected into output string</param>
        /// <returns>Copy of pattern string with injected values</returns>
        template<typename... TArgs>
        std::string format(const TArgs&... args)
        {
            auto sout = std::ostringstream();
            auto size = _pattern.size() + 1;
            auto buffer = std::unique_ptr<char[]>(new char[size]);
            std::memcpy(buffer.get(), _pattern.c_str(), size);
            step(sout, buffer.get(), args...);
            return sout.str();
        }
    };



    /// <summary>
    /// Shorthand for `StringFormatter(pattern)`
    /// </summary>
    StringFormatter operator"" _sf(const char* pattern, size_t size)
    {
        return StringFormatter(std::string(pattern, size));
    }
}

