# Weather-Station-Raspbian  

 ## About
  
Weather Station Application was designed to run on Raspberry Pi. The model used was 3b+. This application was also built with Qt Creator on a Raspberry Pi. An Arduino equipped with the DHT11 temperature/humidity sensor, and a cadmium sulfide photoresistor feeds data into the program via USB. The AccuWeather API is being used to pull weather information from any chosen location.

## Install Qt on Raspberry Pi

1 sudo apt-get update  
2 sudo apt-get upgrade  
3 sudo apt-get install qt5-default  
4 sudo apt-get install qtcreator  
5. Open Qtcreator and go to Help > about plugins and and untick Remote Linux  
6. Restart your raspberry pi  
7. Again open Qt creator, go to "Tools > Options > Build and Run" and go to Compilers. Add "GCC" set compiler path as /usr/bin/gcc  
8. Normally Qt version and Debugger will be automatically set, but you can also set them  
9. Then go to Kits (Still inside the Build and Run) and check whether Compiler, Debugger and Qt version are set  
10. That's it, click Ok and create a new project or open a project  

![wsls](https://user-images.githubusercontent.com/22214754/76711293-ea172100-66cb-11ea-9369-ef36785cfc6e.png)  

![DHT11_Diagram](https://user-images.githubusercontent.com/22214754/170851788-415b0d6e-bf3a-4849-b906-614d9988d657.png)  
