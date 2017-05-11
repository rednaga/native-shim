# native-shim
A "shim" for loading native jni files for Android active debugging

![Debugger Settings](/resources/debugger_settings.png?raw=true)

Purpose
----------
Instead of setting up multiple debuggers, launch the `native-shim` and point your debugger (gdb/IDA/etc) at it and force it to load what ever functions would normally be invoked from the Dalvik code (or aren't... but you want to step into them).


Disclaimer
----------

This code is meant for education and research purposes only. Do as you please with it, but accept any and all responsibility for your actions. The tools were created specifically to assist in malware reversing and analysis - be careful.


Usages
------

* [Native Harness Target](https://github.com/CalebFenton/native-harness-target) - Call native functions from the command line


License
-------

    Copyright 2015 Tim 'diff' Strazzere <strazz@gmail.com>

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.