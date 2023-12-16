import 'package:flutter/material.dart';
import 'package:weekday_selector/weekday_selector.dart';

class MyWeekdaySelector extends StatelessWidget {
  void Function(int)? onChanged;
  List<bool?> values;

  MyWeekdaySelector({super.key, required this.onChanged, required this.values});

  @override
  Widget build(BuildContext context) {
    return WeekdaySelector(
      onChanged: onChanged,
      values: values,
      firstDayOfWeek: 0,
      shortWeekdays: const ["D", "S", "T", "Q", "Q", "S", "S"],
      weekdays: const [
        "Domingo",
        "Segunda",
        "Terça",
        "Quarta",
        "Quinta",
        "Sexta",
        "Sábado"
      ],
    );
  }
}
