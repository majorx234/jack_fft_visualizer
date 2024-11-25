# Info 
- Example Application for Jack
  - show audio capturing
  - in combination with GUI
- creates a spectrum visualization out of audio stream
- makes use of Raylib and Jack Audio Connection Kit

# TODO
- adjust simple_dft to have values between 0 and 1 with logarithmic scale
- replace simple_dft with fft
- documentation
  - overview of modules and  data flow
- unix signal handling
- notify mechanism signal modules new data, closeing

# References
- "if that's a theft than that's art" Quote by Tsoding
- Music Visiualizer Project by Tsoding: 
  - https://www.youtube.com/watch?v=Xdbk1Pr5WXU&list=PLpM-Dvs8t0Vak1rrE2NJn8XYEJ5M7-BqT
- thx to toxypiks for explaination (helps me a lot to understand the code):
  - https://github.com/toxypiks/music_visualizer/
- Raylib: https://www.raylib.com/
- Jack: https://jackaudio.org/
  - https://github.com/jackaudio/jack-example-tools/blob/main/example-clients/capture_client.c
