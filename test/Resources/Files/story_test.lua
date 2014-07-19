--$<atlas,z_test.plist,z_test.png
--$<image,grossini.png
--$<arm,Cowboy.ExportJson
-- set desired design size
winsize(720, 480)

-- bg color
bgcolor(0xff330340)

-- create a label
-- set its color, anchor and position relative to screen
labelsize(36)
labelfont('Courier')
label('demo', 'Story Demo', 0, 0)
color('demo', 0xffff33ff)
anchor('demo', 0, 0)
spos('demo', 0, 0)

-- create an armature
-- set its pos to screen center
-- set it scale and play Walk animation
arm('cowboy', 'Cowboy', 0, 0)
spos('cowboy', 0.5, 0.5)
scale('cowboy', 0.2, 0.2)
armplay('cowboy', 'Walk')

-- image from single file
img('grossini', 'grossini.png', 40, 260)

-- image from atlas
img('flag', "usa_flag.jpg", 40, 340)

-- wait user click, hint user
-- remove the hint after click
label('hint', 'Now Click to Fire', 0, 0)
anchor('hint', 1, 0)
spos('hint', 1, 0)
blink('hint', 10000, 10000)
waitclick()
remove('hint')

-- after click, fire
armplay('cowboy', 'Fire')

-- wait Fire animation, then back to walk
waitarm()
armplay('cowboy', 'Walk')

-- set default msg and name options
msgsize(44)
msgcolor(0xff097788)
msgpos(360, 100)
namepos(360, 400)
namecolor(0xff995467)
namesize(44)

-- show message
msg('Cowboy', 'Hooray, I killed you')