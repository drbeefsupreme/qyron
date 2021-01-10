from __main__ import app, interface
from flask import request, render_template, flash

#form magic
from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, BooleanField, SubmitField, FormField
from wtforms.validators import DataRequired

import cryptocompare

layerStart = {1: interface.scrollingLayer1_start,
              2: interface.scrollingLayer2_start,
              3: interface.scrollingLayer3_start,
              4: interface.scrollingLayer4_start,
              5: interface.scrollingLayer5_start,
              6: interface.scrollingLayerF_start}

layerSpeed = {1: interface.scrollingLayer1_speed,
              2: interface.scrollingLayer2_speed,
              3: interface.scrollingLayer3_speed,
              4: interface.scrollingLayer4_speed,
              5: interface.scrollingLayer5_speed,
              6: interface.scrollingLayerF_speed}

class LayerForm(FlaskForm):
    layerText = StringField('text')
    layerSpeed = StringField('speed')
    submit = SubmitField('submit')

#dynamic form for layers
def DLayerForm(arg):
    class TempForm(FlaskForm):
        text = StringField()
        speed = StringField()
    setattr(TempForm, 'layerText', StringField(arg))
    setattr(TempForm, 'layerSpeed', StringField(arg))
    return TempForm


arg_list = ['one', 'two', 'three', 'four', 'five']

# class BigForm(FlaskForm):
#     def __init__(self):
#         for i in range(5):
#             self.FormField(DLayerForm(arg=arg_list[i]))


class BigDumbForm(FlaskForm):
    layerText1 = StringField('layer 1 text')
    layerText2 = StringField('layer 2 text')
    layerText3 = StringField('layer 3 text')
    layerText4 = StringField('layer 4 text')
    layerText5 = StringField('layer 5 text')
    layerSpeed1 = StringField('layer 1 speed')
    layerSpeed2 = StringField('layer 2 speed')
    layerSpeed3 = StringField('layer 3 speed')
    layerSpeed4 = StringField('layer 4 speed')
    layerSpeed5 = StringField('layer 5 speed')

    abortBit = SubmitField('ABORT BIT')
    legallyActionable = SubmitField('Not Legally Actionable')
    parody = SubmitField('PARODY')

    featureDemo = SubmitField('demo')
    randomShapes = SubmitField('randomShapes')
    randomPixels = SubmitField('randomPixels')
    cryptoPrices = SubmitField('cryptoPrices')

    setRedBG = SubmitField('red bg')
    setBlackBG = SubmitField('black bg')
    setBlinking = SubmitField('flashing')

    submit = SubmitField('submit')
    clear = SubmitField('clear')



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
    formTemplates = [DLayerForm(arg=arg_list[i]) for i in range(5)]
    forms = [formTemplates[i]() for i in range(5)]
    currentText = ["" for i in range(5)]
    currentSpeed = ["" for i in range(5)]
    for i in range(5):
        if forms[i].validate_on_submit():
            flash('Parameters submitted - text={}, speed={}'.format(forms[i].layerText.data, forms[i].layerSpeed.data))
            currentText[i] = str(forms[i].layerText.data)
            currentSpeed[i] = str(forms[i].layerSpeed.data)
    for i in range(5):
        if currentText[i] is not None and currentText[i] is not "":
            layerStart[i+1](currentText[i].encode('utf-8'), -1)
    return render_template('layerform.html', title='Layers', form=forms[0])

@app.route('/dumb', methods=['GET', 'POST'])
def dumb():
    form = BigDumbForm()
    currentText = ["" for i in range(6)]
    currentSpeed = ["" for i in range(6)]
    if form.validate_on_submit():
        if form.submit.data:
            currentText[0] = str(form.layerText1.data)
            currentText[1] = str(form.layerText2.data)
            currentText[2] = str(form.layerText3.data)
            currentText[3] = str(form.layerText4.data)
            currentText[4] = str(form.layerText5.data)
            currentSpeed[0] = str(form.layerSpeed1.data)
            currentSpeed[1] = str(form.layerSpeed2.data)
            currentSpeed[2] = str(form.layerSpeed3.data)
            currentSpeed[3] = str(form.layerSpeed4.data)
            currentSpeed[4] = str(form.layerSpeed5.data)
        if form.abortBit.data:
            currentText[0] = "ABORT BIT ABORT BIT ABORT BIT ABORT BIT"
            currentText[1] = "ABORT BIT ABORT BIT ABORT BIT ABORT BIT"
            currentText[2] = "ABORT BIT"
            currentText[3] = "ABORT BIT"
            currentText[4] = "ABORT BIT"
            currentSpeed[0] = "60"
            currentSpeed[1] = "150"
            currentSpeed[2] = "25"
            currentSpeed[3] = "90"
            currentSpeed[4] = "10"
        if form.parody.data:
            currentText[0] = "PARODY PARODY PARODY PARODY PARODY"
            currentText[1] = "IN MINECRAFT IN MINECRAFT IN MINECRAFT"
            currentText[2] = "PARODY ALERT PARODY ALERT"
            currentText[3] = "PARODY"
            currentText[4] = "MINECRAFT"
            currentSpeed[0] = "75"
            currentSpeed[1] = "160"
            currentSpeed[2] = "50"
            currentSpeed[3] = "100"
            currentSpeed[4] = "120"
        if form.legallyActionable.data:
            currentText[0] = "!!! NOT LEGALLY ACTIONABLE !!! NOT LEGALLY ACTIONABLE !!! NOT LEGALLY ACTIONABLE !!! NOT LEGALLY ACTIONABLE  !!!"
            currentText[1] = "IN MINECRAFT, FAKE, PARODY, FAKE, IN MINECRAFT, IN MINECRAFT, IN MINECRAFT"
            currentText[2] = "IN THE MATRIX"
            currentText[3] = "PARODY"
            currentText[4] = "PARODY"
            currentSpeed[0] = "50"
            currentSpeed[1] = "70"
            currentSpeed[2] = "130"
            currentSpeed[3] = "15"
            currentSpeed[4] = "150"
        if form.clear.data:
            for i in range(6):
                currentText[i] = " "
        if form.featureDemo.data:
            interface.runFeatureDemo()
        if form.randomShapes.data:
            interface.drawRandomShapes()
        if form.randomPixels.data:
            interface.drawRandomPixels()
        if form.cryptoPrices.data:
            prices = cryptocompare.get_price(['BTC','ETH', 'XMR', 'DOGE', 'HOT'],curr='USD')
            priceString = "UP TO THE MINUTE CRYPTOCURRENCY PRICES. BROUGHT TO YOU BY CROWCOIN, THE OFFICIAL SHITCOIN OF THE TOO LATE SHOW"
            priceString = priceString + "   BTC: $" + str(prices["BTC"]["USD"])
            priceString = priceString + "   ETH: $" + str(prices["ETH"]["USD"])
            priceString = priceString + "   XMR: $" + str(prices["XMR"]["USD"])
            priceString = priceString + "   DOGE: $" + str(prices["DOGE"]["USD"])
            priceString = priceString + "   HOT: $" + str(prices["HOT"]["USD"])
            priceString = priceString + "   URBIT PLANET: $35.52     URBIT STAR: $4653.51"
            currentText[0] = priceString
        if form.setBlackBG.data:
            interface.setBlackBackground()
        if form.setRedBG.data:
            interface.setRedBackground()
        if form.setBlinking.data:
            interface.toggleBlinking()

    for i in range(6):
        if currentText[i] is not None and currentText[i] is not "":
            layerStart[i+1](currentText[i].encode('utf-8'), -1)
        if currentSpeed[i] is not None and currentSpeed[i] is not "":
            layerSpeed[i+1](int(currentSpeed[i]))

    return render_template('dumbform.html', title='Layers', form=form)


@app.route("/layer1")
def layer1text():
    interface.scrollingLayer1_start(b'Layer 1 test', -1)

@app.route("/layer2")
def layer2text():
    interface.scrollingLayer2_start(b'layer 2 test', -1)
