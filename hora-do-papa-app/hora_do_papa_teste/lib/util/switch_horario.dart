import 'package:flutter/material.dart';

class SwitchHorario extends StatefulWidget {
  bool ativo;
  
  SwitchHorario({super.key,
                 required this.ativo});
  
  @override
  State<SwitchHorario> createState() => _SwitchHorarioState();
}

class _SwitchHorarioState extends State<SwitchHorario> {
  @override
  Widget build(BuildContext context) {
    return Switch(
      value: widget.ativo,
      activeColor: Colors.green,
      onChanged: (bool troca) {
        setState(() {
          widget.ativo = troca;
        });
      },
    );
  }
}