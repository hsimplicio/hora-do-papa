import 'package:flutter/material.dart';
import '../util/card_horario.dart';
import '../util/new_time_box.dart';
import '../util/edit_time_box.dart';

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key});

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  // text controller
  final _controllerHour = TextEditingController();
  final _controllerMinute = TextEditingController();
  List<bool> diasList = List.filled(7, false);

  // list of horarios
  List horariosLista = [
    [
      "06",
      "30",
      [true, true, true, true, true, false, false],
      true
    ],
    [
      "08",
      "00",
      [false, false, false, false, false, true, true],
      true
    ],
    [
      "12",
      "15",
      [false, false, false, true, false, false, false],
      false
    ],
  ];

  // create a new time
  void createNewTime() {
    showDialog(
      context: context,
      builder: (context) {
        return NewTimeBox(
          controllerHour: _controllerHour,
          controllerMinute: _controllerMinute,
          diasList: diasList,
          onSave: saveNewTime,
          onCancel: cancelTime,
        );
      },
    );
  }

  void saveNewTime() {
    setState(() {
      horariosLista
          .add([_controllerHour.text, _controllerMinute.text, diasList, true]);
      _controllerHour.clear();
      _controllerMinute.clear();
      diasList = List.filled(7, false);
    });
    Navigator.of(context).pop();
  }

  void cancelTime() {
    setState(() {
      _controllerHour.clear();
      _controllerMinute.clear();
      diasList = List.filled(7, false);
    });
    Navigator.of(context).pop();
  }

  // edit existent time

  void editTime(int index) {
    diasList = List.from(horariosLista[index][2]);
    showDialog(
      context: context,
      builder: (context) {
        return EditTimeBox(
          controllerHour: _controllerHour,
          controllerMinute: _controllerMinute,
          hora: horariosLista[index][0],
          minuto: horariosLista[index][1],
          diaSemana: diasList,
          onSave: () => saveEditedTime(index),
          onCancel: cancelTime,
          onDelete: () => deleteTime(index),
        );
      },
    );
  }

  void saveEditedTime(int index) {
    setState(() {
      if (_controllerHour.text != "") {
        horariosLista[index][0] = _controllerHour.text;
      }
      if (_controllerMinute.text != "") {
        horariosLista[index][1] = _controllerMinute.text;
      }
      horariosLista[index][2] = diasList;
      _controllerHour.clear();
      _controllerMinute.clear();
      diasList = List.filled(7, false);
    });
    Navigator.of(context).pop();
  }

  void deleteTime(int index) {
    setState(() {
      horariosLista.removeAt(index);
      _controllerHour.clear();
      _controllerMinute.clear();
      diasList = List.filled(7, false);
    });
    Navigator.of(context).pop();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Horários Salvos'),
        elevation: 0,
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: createNewTime,
        child: const Icon(Icons.add),
      ),
      floatingActionButtonLocation: FloatingActionButtonLocation.centerFloat,
      bottomNavigationBar: NavigationBar(
        destinations: const <Widget>[
          NavigationDestination(
            icon: Icon(Icons.alarm),
            label: 'Salvos',
          ),
          NavigationDestination(
            icon: Icon(Icons.pets_rounded),
            label: 'Ativar',
          ),
        ],
      ),
      body: ListView.builder(
        itemCount: horariosLista.length,
        itemBuilder: (context, index) {
          return CardHorario(
            hora: horariosLista[index][0],
            minuto: horariosLista[index][1],
            diasSemana: horariosLista[index][2],
            ativo: horariosLista[index][3],
            onPressed: () => editTime(index),
          );
        },
      ),
    );
  }
}
