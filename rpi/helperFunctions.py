class HelperFunctions:
    def __init__(self, options=None):
        self.options = options

    def get_verbose(self):
        return self.options['verbose']

    def get_options(self):
        return self.options

def print_verbose(text, indent=0):
#    if HelperFunctions().get_options()['verbose']:
     print('\t'*indent, end='')
     print(text)
