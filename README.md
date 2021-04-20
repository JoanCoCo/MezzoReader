# MezzoReader
 Project based on OpenCV and OpenAL aiming to recognize, read and play musical notation from images. This project was developed on macOS 11.2.3 for SMII 2020/2021 at Universitat Politècnica de València.
 
 ## Installation
 ### macOS
 MezzoReader uses the OpenAL framework that it is already installed by default on the official macOS distribution. However, it is necessary to install the OpenCV 4.2.0 library **on the project folder**. For this purpose follow the next steps:
 1. Make sure that **cmake** command line tool is installed on your machine. If it is not, you can install it by downlading the latest Unix source from the official [CMake Organization webpage](https://cmake.org/download/). Once downloaded, you can install it by executing on the downloaded folder `$ ./configure`, followed by `$ make` and `$ sudo make install`.
 2. Make sure that **Xcode** is installed on your machine. If it is not you can install the latest version from the [App Store](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwj4ju7fnYrwAhXRbsAKHWJhAW4QFjAAegQIAxAD&url=https%3A%2F%2Fapps.apple.com%2Fes%2Fapp%2Fxcode%2Fid497799835%3Fmt%3D12&usg=AOvVaw2hyaQ-p-FBilgfTLM1S46t).
 3. Install Xcode's command line tools by executing on the shell the command `$ xcode-select --install`.
 4. Finally, install the dependencies of the project. For that, execute on the project folder the command `$ make dependencies`. **IMPORTANT:** it is required for this command to work properly that the path where you saved the project folder **doesn't contain wite spaces**. Take in account that this command will proceed to download the OpenCV 4.2.0 library and install it on the project folder, so it can take a while. If something fails during the installation, remove the **tmp** and **dependencies** folders inside the project folder before trying to execute the command again.
 
 ### Linux
 MezzoReader uses the OpenAL SDK that can be installed on the system through the official distribution Linux. It is also necessary to install the OpenCV 4.2.0 library **on the project folder**. For this purpose you can execute the command `$ make dependencies` on the project folder. This command will verify that the OpenAL SDK is installed, and will install it if it is not (requieres superuser privileges). It also will proceed to download the OpenCV 4.2.0 library and install it on the project folder, so it can take a while. **IMPORTANT:** it is required for this command to work properly that the path where you saved the project folder **doesn't contain wite spaces**. If something fails during the installation, remove the **tmp** and **dependencies** folders inside the project folder before trying to execute the command again.
 
 ## Compilation
 Once the dependencies have been correctly installed, MezzoReader can be compiled by executing on the project folder the command `$ make`.
 
 ## Use
 MezzoReader's interface can be consulted by executing `$ ./MezzoReader -h`, and it corresponds with the following extract:
 ```
 Usage: MezzoReader [params] 

     -?, -h, --help, --usage (value:true)
         Usage examples: 
         ./MezzoReader -i=images/notes.png -o=analysed_image.png -p -v 
         ./MezzoReader -a=2 -ap=5
     -a, --staffs (value:<none>)
         Adaptive mode given the number of staffs
         contained in the image.
     -d, --decode (value:<none>)
         Load a melody from a pictoform
     -e, --encode (value:<none>)
         Save the extracted melody as pictoform.
     -i, --input (value:images/notation.png)
         Input image.
     -n, --precision (value:3)
         Precision for the adaptive mode as 10e-n.
     -o, --output (value:pentagrama_analizado.png)
         Output image.
     -p, --play (value:<none>)
         Play music after the analysis.
     -v, --visual (value:<none>)
         Visual mode.
 
 ```
 With the `-i` parameter you can provide and image to be analyzed by MezzoReader. The parameter `-v` will allow seing the analysis process in real time, while the `-p` option will allow the system to play the found melody after the analysis.
 
 Options `-a` and `-n` are used for the so called *adaptive mode*. In this mode, the parameter `-a` indicates how many staff the provided music sheet image contains, and the system will try to adapt its internal parameters to found the given number of staffs in the image. This option is recommended when the system can't find the staffs inside an image with the normal parameters. However, it must be taken in account that on the process of trying to adapt MezzoReader's paramaters that accuracy of the system can be highly lowered down and that *adaptive mode* doesn't ensure that the given number of staffs will be found.
 
 Finally, options `-e` and `-d` are used for dealing with *pictoforms*. *Pictoforms* are images that codify the notes and silences that form a monophonic melody. MezzoReader can write the recognized melody into a *pictoform* by using the `-e` option and can read and play the sounds contained on a *pictoform* by using the `-d` parameter. Take in account that the `-d` parameter has preference over the rest of the parameters, causing the system to ignore all the others when `-d` is present.
 
 ## Limitations
 Currently the system is design for recognizing monophonic staffs in treble clef and with a 4 by 4 beat. Moreover, in the current version it just supports the following symbols:
 * Crotchet note.
 * Minim note.
 * Semibreve note.
 * Quaver note.
 * Crotchet silence.
 * Minim silence.
 * Semibreve silence.
 
 ## Compatibility
 Compatibility has been tested with macOS 11 Big Sur running on an Intel based mac. Compatibility with Apple Silicon based mac has not been tested.
 
 MezzoReader is also compatible with Linux systems. However, and although this compatibility has been brievly tested, it is recommended its use on macOS systems due to it has the development platform for the project.
