import 'package:flutter/material.dart';

import 'my_button.dart';
import 'my_weekday_selector.dart';

class NewTimeBox extends StatefulWidget {
  final dynamic controllerHour;
  final dynamic controllerMinute;
  List<bool> diasList = List.filled(7, false);
  VoidCallback onSave;
  VoidCallback onCancel;

  NewTimeBox(
      {super.key,
      required this.controllerHour,
      required this.controllerMinute,
      //required this.controllerDias,
      required this.diasList,
      required this.onSave,
      required this.onCancel});

  @override
  State<NewTimeBox> createState() => _NewTimeBoxState();
}

class _NewTimeBoxState extends State<NewTimeBox> {
  @override
  Widget build(BuildContext context) {
    return AlertDialog(
        title: const Text("Novo Horário"),
        content: Column(
          mainAxisSize: MainAxisSize.min,
          mainAxisAlignment: MainAxisAlignment.start,
          children: [
            // get user input
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Flexible(
                  flex: 2,
                  child: TextField(
                      controller: widget.controllerHour,
                      decoration: const InputDecoration(
                        border: OutlineInputBorder(),
                        hintText: "HH",
                      ),
                      style: const TextStyle(fontSize: 20)),
                ),
                const Flexible(
                  flex: 2,
                  child: SizedBox(
                    width: 10,
                    child: Center(
                        child: Text(":", style: TextStyle(fontSize: 20))),
                  ),
                ),
                Flexible(
                  flex: 2,
                  child: TextField(
                      controller: widget.controllerMinute,
                      decoration: const InputDecoration(
                        border: OutlineInputBorder(),
                        hintText: "MM",
                      ),
                      style: const TextStyle(fontSize: 20)),
                ),
              ],
            ),

            //const Spacer(),
            const SizedBox(height: 8),

            // TextField(
            //   controller: controllerDias,
            //   decoration: const InputDecoration(
            //     border: OutlineInputBorder(),
            //     hintText: "STQQSSD",
            //   ),
            // ),

            MyWeekdaySelector(
              onChanged: (int day) {
                final index = day % 7;
                setState(() {
                  widget.diasList[index] = !widget.diasList[index];
                });
              },
              values: widget.diasList,
            ),

            //const Spacer(),
            const SizedBox(height: 16),

            // buttons -> save + cancel
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceEvenly,
              children: [
                // save button
                Flexible(
                  flex: 2,
                  child: MyButton(
                    name: "Cancelar",
                    onPressed: widget.onCancel,
                  ),
                ),

                //const SizedBox(width: 8),

                // cancel button
                Flexible(
                  flex: 2,
                  child: MyButton(
                    name: "Adicionar",
                    onPressed: widget.onSave,
                  ),
                ),
              ],
            ),
          ],
        ));
  }
}
