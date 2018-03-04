# scanfix
Openframeworks app to edit slitscans

Move the y location of an image along a source curve to align with a destination curve. You can edit, insert or remove points, as well as toggle between the original and the aligned image. You can also pan around and zoom in the image

There is a gui to help you.

Getting started:
<ul>
<li>run the app
<li>when it opens, select "new"
<li>select a slit scan image and load it.
<li>you will see two curves drawn on it, one flat, one wiggly
<li>press the space bar and see the y location at the wiggly curve align to the straight line.
<li>press "save"
<li>an .xml file will be created next to your image, same name but with xml extention
<li>press "open" to load an existing xml file. It will contain a reference to the original image as well as the curve info.
</ul>
