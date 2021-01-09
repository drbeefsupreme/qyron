from __main__ import app, interface
from flask import request

@app.route("/", methods=["GET", "POST"])
def index():
    errors = ""
    if request.method == "POST":
        layer1 = None
        layer2 = None
        try:
            layer1 = str(request.form["layer1"])
        except:
            errors += "<p>{!r} is not a string.</p>\n".format(request.form["layer1"])
        try:
            layer2 = str(request.form["layer2"])
        except:
            errors += "<p>{!r} is not a string.</p>\n".format(request.form["layer2"])
        if layer1 is not None and layer1 is not "":
            interface.scrollingLayer1_start(layer1.encode('utf-8'), -1)
        if layer2 is not None and layer2 is not "":
            interface.scrollingLayer2_start(layer2.encode('utf-8'), -1)

    return '''
        <html>
            <body>
                {errors}
                <p>Enter your layers:</p>
                <form method="post" action=".">
                    <p><input name="layer1" /></p>
                    <p><input name="layer2" /></p>
                    <p><input type="submit" value="Set layers" /></p>
                </form>
            </body>
        </html>
    '''.format(errors=errors)


@app.route("/layer1")
def layer1text():
    interface.scrollingLayer1_start(b'Layer 1 test', -1)

@app.route("/layer2")
def layer2text():
    interface.scrollingLayer2_start(b'layer 2 test', -1)
