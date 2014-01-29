Computer Vision
=========
Computer Vision application for linux, using openCV framework to create time lapse video

the application will run and capture image every minute into images folder
then run command such as this to convert into the video

mencoder "mf://*.png" -mf type=png:w=640:h=480:fps=25 -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -oac copy -o output_25.avi
