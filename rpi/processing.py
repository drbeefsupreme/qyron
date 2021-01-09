from __main__ import app, interface
from flask import request, render_template, flash

#form magic
from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, BooleanField, SubmitField
from wtforms.validators import DataRequired

layerStart = {1: interface.scrollingLayer1_start,
              2: interface.scrollingLayer2_start,
              3: interface.scrollingLayer3_start,
              4: interface.scrollingLayer4_start,
              5: interface.scrollingLayer5_start}

layerSpeed = {1: interface.scrollingLayer1_speed,
              2: interface.scrollingLayer2_speed,
              3: interface.scrollingLayer3_speed,
              4: interface.scrollingLayer4_speed,
              5: interface.scrollingLayer5_speed}

class LayerForm(FlaskForm):
    layerText = StringField('text')
    layerSpeed = StringField('speed')
    submit = SubmitField('submit')

#dynamic form for layers
def DLayerForm(arg):
    class TempForm(FlaskForm):
        text = StringField()
        speed = StringField()
        submit = SubmitField()
    setattr(TempForm, 'text', StringField(arg))
    setattr(TempForm, 'speed', StringField(arg))
    setattr(TempForm, 'submit', SubmitField(arg))
    return TempForm

arg_list = ['one', 'two', 'three', 'four', 'five']
form_list = [DLayerForm(arg=arg_list[i]) for i in range(5)]


@app.route("/alllayers", methods=["GET", "POST"])
def allLayers():
    errors = ""
    layerText = ["" for i in range(5)]
    if request.method == "POST":
        for i in range(5):
            try:
                layerText[i] = str(request.form["layer"+str(i+1)])
            except:
                errors += "<p>{!r} is not a string.</p>\n".format(request.form["layer"+str(i+1)])
            if layerText[i] is not None and layerText[i] is not "":
                layerStart[i+1](layerText[i].encode('utf-8'), -1)

    return render_template('alllayers.html', errors=errors)

@app.route('/', methods=['GET', 'POST'])
def index():
    form = [LayerForm() for i in range(5)]
    currentText = ["" for i in range(5)]
    currentSpeed = ["" for i in range(5)]
    if form[i].validate_on_submit():
        flash('Parameters submitted - text={}, speed={}'.format(form[i].layerText.data, form[i].layerSpeed.data))
        currentText[i] = str(form[i].layerText.data)
        currentSpeed[i] = str(form[i].layerSpeed.data)
    for i in range(5):
        if currentText[i] is not None and currentText[i] is not "":
            layerStart[i+1](currentText[i].encode('utf-8'), -1)
    return render_template('layerform.html', title='Layers', form=form)


@app.route("/layer1")
def layer1text():
    interface.scrollingLayer1_start(b'Layer 1 test', -1)

@app.route("/layer2")
def layer2text():
    interface.scrollingLayer2_start(b'layer 2 test', -1)
