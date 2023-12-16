import 'package:flutter/material.dart';

import 'my_button.dart';
import 'my_weekday_selector.dart';

class EditTimeBox extends StatefulWidget {
  final dynamic controllerHour;
  final dynamic controllerMinute;
  final String hora;
  final String minuto;
  final List<bool> diaSemana;
  VoidCallback onSave;
  VoidCallback onCancel;
  VoidCallback onDelete;

  EditTimeBox(
      {super.key,
      required this.controllerHour,
      required this.controllerMinute,
      //required this.controllerDias,
      required this.hora,
      required this.minuto,
      required this.diaSemana,
      required this.onSave,
      required this.onCancel,
      required this.onDelete});

  @override
  State<EditTimeBox> createState() => _EditTimeBoxState();
}

class _EditTimeBoxState extends State<EditTimeBox> {
  @override
  Widget build(BuildContext context) {
    return AlertDialog(
        title: const Text("Editar Horário"),
        content: Column(
          mainAxisSize: MainAxisSize.min,
          mainAxisAlignment: MainAxisAlignment.start,
          children: [
            Card(
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(8),
              ),
              child: Padding(
                padding: const EdgeInsets.only(
                    left: 20, right: 15, top: 10, bottom: 10),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.start,
                  children: [
                    Text(
                      "${widget.hora}:${widget.minuto}",
                      style: const TextStyle(fontSize: 26),
                    ),
                    // Expanded(
                    //     child: Center(
                    //   child: IgnorePointer(
                    //     child: MyWeekdaySelector(
                    //       onChanged: (int day) {},
                    //       values: widget.horario[2],
                    //     ),
                    //   ),
                    // )),
                  ],
                ),
              ),
            ),

            const SizedBox(height: 8),

            // get user input
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Flexible(
                  flex: 2,
                  child: TextField(
                      controller: widget.controllerHour,
                      decoration: InputDecoration(
                        border: const OutlineInputBorder(),
                        hintText: widget.hora,
                      ),
                      style: const TextStyle(fontSize: 20)),
                ),
                const Flexible(
                  flex: 2,
                  child: SizedBox(
                    width: 10,
                    child: Center(child: Text(":")),
                  ),
                ),
                Flexible(
                  flex: 2,
                  child: TextField(
                      controller: widget.controllerMinute,
                      decoration: InputDecoration(
                        border: const OutlineInputBorder(),
                        hintText: widget.minuto,
                      ),
                      style: const TextStyle(fontSize: 20)),
                ),
              ],
            ),

            //const Spacer(),
            const SizedBox(height: 8),

            MyWeekdaySelector(
              onChanged: (int day) {
                final index = day % 7;

                setState(() {
                  widget.diaSemana[index] = !widget.diaSemana[index];
                });
              },
              values: widget.diaSemana,
            ),

            //const Spacer(),
            const SizedBox(height: 16),

            // buttons -> save + cancel
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                // delete button
                Flexible(
                  flex: 2,
                  child: MyButton(
                    name: "Excluir",
                    onPressed: widget.onDelete,
                  ),
                ),

                // cancel button
                Flexible(
                  flex: 2,
                  child: MyButton(
                    name: "Cancelar",
                    onPressed: widget.onCancel,
                  ),
                ),

                //const SizedBox(width: 8),

                // save button
                Flexible(
                  flex: 2,
                  child: MyButton(
                    name: "Salvar",
                    onPressed: widget.onSave,
                  ),
                ),
              ],
            ),
          ],
        ));
  }
}
