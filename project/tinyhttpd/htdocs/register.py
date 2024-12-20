#!/home/xs/anaconda3/bin/python3

import sys, os
length = os.getenv('CONTENT_LENGTH')
if length:
    postdata = sys.stdin.read(int(length))  # 从子进程读端口 读取length字节长度的payload数据
    print("Content-type:text/html\n")
    print('<html>')
    print('<head>')
    print('<title>POST</title>')
    print('</head>')
    print('<body>')
    print('<h2> POST data </h2>')
    print('<ul>')
    for data in postdata.split('&'):
        print('<li>' + data + '</li>')
    print('</ul>')
    print('</body>')
    print('</html>')
else:
    print("Content-type:text/html\n")
    print('no found')