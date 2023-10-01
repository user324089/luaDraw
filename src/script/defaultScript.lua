local constructions = require "constructions"

local testCirc = constructions.testFunc()
print (testCirc.x, testCirc.y, testCirc.r)
print ("type of testCirc is ", type(testCirc))

local pt = luaDraw.newPoint();
print('printing pt.x', pt.x)
pt.x = 1.3;
print('printing pt.x', pt.x)

local pt2 = luaDraw.newPoint();
print('printing pt2.x', pt2.x)
pt2.x = -3.2;
print('printing pt2.x', pt2.x)

local l1 = luaDraw.newLine();
l1.a = 1
l1.b = 0.1
l1.c = 0

local c1 = luaDraw.newCircle()
c1.x = 1
c1.y = 1
c1.r = 4

local col = luaDraw.colors.newColor ()
col.r = 0.3
col.g = 0.7
c1.color = col

pt.color = luaDraw.colors.brown

function Update ()
    c1.x = luaDraw.getTime();
    c1.y = luaDraw.getTime();
    c1.r = luaDraw.getTime();
    local pt3 = luaDraw.newPoint ();
    pt3.x = c1.x;
    pt3.y = c1.y;
end
