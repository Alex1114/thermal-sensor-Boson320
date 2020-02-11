# thermal_sensor
Uses v4l2 to capture images from the FLIR camera. Processes the images with OpenCV and publishes them using cv_bridge.
```
Camera:Boson320
```
## Boson_USB
This is an example of capturing Boson Video (USB) using V4L2 (Video for Linux 2)
and OpenCV to display the video. Boson USB has two modes, AGC-8 bits and
RAW-16bits.
Video in 16 bits is not paintable so a linear transformation needs to happen
before displaying the image. We call this process AGC, and in this example we
use the most basic one (LINEAR). Then we use stardard call to paint in GREY.
Video is 8 bits is directly paintable, linear transformation happens inside the
 camera, not further actions need to happen in SW.
To Display Boson data we are using OpenCV to convert from YUV to RGB.

### Demo
![BosonUSB y](figures/BosonUSB y.png)
![BosonUSB r](figures/BosonUSB r.png)

## cali_thermal

## flir_boson_usb

## sync_img

