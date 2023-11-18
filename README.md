# vocal-distortion
Sound Processing - Mini Project - Vocal Distortion and real-time processing of roughness

This project is a real-time implementation of a vocal distortion and roughness effect, described in detail in a paper by Gentilucci, Marta, Luc Ardaillon, and Marco Liuni [^1]. The effect was implemented as an audio effect plugin using the JUCE framework. 


### How to run this project
Download the demo release. Unpack the zip file and copy the desired plugin type to your plug-ins' directory.  
Default macOS audio plug-ins paths:  
- AU: `/Library/Audio/Plug-Ins/Components`  
- VST3: `/Library/Audio/Plug-Ins/VST3`

**The demo release is compatible with macOS 10.13+ both M1 and Intel machines. For any other systems it is required to build an executable by yourself.**

[^1]: Marta Gentilucci, Luc Ardaillon, and Marco Liuni. Vocal distortion and
real-time processing of roughness. International Computer Music Conference
Proceedings, 2018, 2018. ISSN 2223-3881.
