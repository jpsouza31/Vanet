'''
Created on Feb 4, 2013

@author: Guilherme
'''

# Buildings occupying the whole blocks
#x_start = 4.75
#y_start = 4.75

#x_length = 90.49
#y_length = 90.49

#x_delta = 9.51
#y_delta = 9.51


# Buildings occupying 80% of the blocks
x_start = 9.75
y_start = 9.75

x_length = 80.49
y_length = 80.49

x_delta = 19.51
y_delta = 19.51

counter = 0

for i in range(9):
    for j in range(9):
        x_begin = 0.0
        x_end = 0.0
        y_begin = 0.0
        y_end = 0.0
                
        x_begin = (j * x_delta) + (j * x_length) + x_start
        x_end = x_begin + x_length
            
        y_begin = (i * y_delta) + (i * y_length) + y_start
        y_end = y_begin + y_length
            
        print str("<poly id=\"building#") + str(counter) + "\" " + \
        str("type=\"building\" color=\"1.0,.0,.0\" fill=\"1\" shape=\"") + \
        str(x_begin) + "," + str(y_begin) + " " + \
        str(x_end) + "," + str(y_begin) + " " + \
        str(x_end) + "," + str(y_end) + " " + \
        str(x_begin) + "," + str(y_end) + " " + \
        str(x_begin) + "," + str(y_begin) + "\" />"
            
        counter += 1        