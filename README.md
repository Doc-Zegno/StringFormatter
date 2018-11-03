# StringFormatter
Native implementation of C# string.Format() analog in C++. 100% Boost free



## How to use
Add file `Include/StringFormatter.h` to your project.

You may want to use a rather convenient shorthand with `_sf` suffix 
instead of fully specified instantiation of `StringFormatter` class:
```
// Will produce... well, you get it right
auto result = "{} is {} times better than ducking {}"_sf.format("Python", 137, "C++");
```



## Additional options
You can also place specific options within placeholder `{}` after a colon:
* `x` for hex integers:
    ```
    "{} dec is 0x{:x} hex"_sf.format(32, 32);
    ```
* Well, I hope the list will become a bit longer one day



## Troubleshooting
Current code requires C++17. If it's not an option for you, you can safely remove
all `constexpr`s after corresponding `if`s



## Pros n' Cons
### Pros
* Boost is not required
* Python-like style
* Performs lots of checks


### Cons
* Lack of useful options so far
* Checks are performed during run-time
    


## License
You can do what the fuck you want with this project
