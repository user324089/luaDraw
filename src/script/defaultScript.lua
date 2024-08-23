local constructions = require "constructions"

local testCirc = constructions.testFunc()
print (testCirc.x, testCirc.y, testCirc.r)
print ("type of testCirc is ", type(testCirc))

local pt = luaDraw.newPoint({x = 1.4, y = 0});
print('printing pt.x', pt.x)

local pt2 = luaDraw.newPoint();
print('printing pt2.x', pt2.x)
pt2.x = -3.2;
print('printing pt2.x', pt2.x)

local l1 = luaDraw.newLine({a=1, b=0.1, c=0})

local c1 = luaDraw.newCircle({x=1, y=1, r=4})

local col = luaDraw.colors.newColor ({r=1,g=0.6,b=0.6})
c1.color = col

pt.color = luaDraw.colors.brown

local horizontal = luaDraw.newLine ({a=0, b = 1, c = 0})
horizontal.color = luaDraw.colors.yellow

local pt4 = luaDraw.newPoint ({x = -1, y = 1})

local linePtPt4 = constructions.lineThroughTwoPoints (pt, pt4);
linePtPt4.color = luaDraw.colors.green

luaDraw.setCallback ('x', function ()
    pt2.x = pt2.x + 1;
end)

function Update ()
    c1.x = luaDraw.getTime();
    c1.y = luaDraw.getTime();
    c1.r = luaDraw.getTime();
    local pt3 = luaDraw.newPoint ();
    pt3.x = c1.x;
    pt3.y = c1.y;
end
