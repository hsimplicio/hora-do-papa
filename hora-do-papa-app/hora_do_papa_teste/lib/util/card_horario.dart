import 'package:flutter/material.dart';
import 'switch_horario.dart';
import 'my_weekday_selector.dart';

class CardHorario extends StatelessWidget {
  String hora;
  String minuto;
  List<bool> diasSemana;
  bool ativo;
  VoidCallback onPressed;

  CardHorario(
      {super.key,
      required this.hora,
      required this.minuto,
      required this.diasSemana,
      required this.ativo,
      required this.onPressed});

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Container(
        padding: const EdgeInsets.only(left: 5, right: 5, top: 5),
        child: Card(
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(8),
          ),
          child: InkWell(
            onTap: onPressed,
            child: Padding(
              padding: const EdgeInsets.only(
                  left: 20, right: 15, top: 10, bottom: 10),
              child: Column(
                mainAxisSize: MainAxisSize.min,
                children: [
                  Row(mainAxisAlignment: MainAxisAlignment.start, children: [
                    Text(
                      "$hora:$minuto",
                      style: const TextStyle(fontSize: 26),
                    ),
                    Expanded(
                      child: SizedBox(
                        height: 26,
                        width: 210,
                        child: Center(
                          child: IgnorePointer(
                            child: MyWeekdaySelector(
                              onChanged: (int day) {},
                              values: diasSemana,
                            ),
                          ),
                        ),
                      ),
                    ),
                    SwitchHorario(ativo: ativo),
                  ]),
                ],
              ),
            ),
          ),
        ),
      ),
    );
  }
}
