import sys
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout


class Screen1(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        self.button = QPushButton('Go to Screen 2')
        self.button.clicked.connect(self.go_to_screen2)
        layout.addWidget(self.button)
        self.setLayout(layout)

    def go_to_screen2(self):
        self.parent().setCurrentIndex(1)


class Screen2(QWidget):
    def __init__(self):
        super().__init__()
        layout = QVBoxLayout()
        self.button = QPushButton('Go to Screen 1')
        self.button.clicked.connect(self.go_to_screen1)
        layout.addWidget(self.button)
        self.setLayout(layout)

    def go_to_screen1(self):
        self.parent().setCurrentIndex(0)


class MainWindow(QWidget):
    def __init__(self):
        super().__init__()
        self.stack = []
        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()

        self.screen1 = Screen1()
        self.screen2 = Screen2()

        self.stack.append(self.screen1)
        self.stack.append(self.screen2)

        self.stacked_widget = QtWidget.QStackedWidget()
        for screen in self.stack:
            self.stacked_widget.addWidget(screen)

        layout.addWidget(self.stacked_widget)
        self.setLayout(layout)

    def setCurrentIndex(self, index):
        self.stacked_widget.setCurrentIndex(index)


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.setWindowTitle('Screen Toggling Example')
    window.setGeometry(100, 100, 400, 300)
    window.show()
    sys.exit(app.exec_())