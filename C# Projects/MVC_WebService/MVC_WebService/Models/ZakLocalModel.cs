using System;
using System.ComponentModel;

namespace MVC_WebService.Models
{
    public class ZakLocalModel
    {
        public Guid Id { get; set; }
        [DisplayName("JMÉNO")]
        public string Jmeno { get; set; }
        [DisplayName("PŘÍJMENÍ")]
        public string Prijmeni { get; set; }
        [DisplayName("JE MUŽ")]
        public bool? Muz { get; set; }
        [DisplayName("RODNÉ ČÍSLO")]
        public string RodneCislo { get; set; }
        [DisplayName("STUDUJE")]
        public bool Studuje { get; set; }
        [DisplayName("TŘÍDA")]
        public Guid? Trida { get; set; }
        [DisplayName("ZMENĚNO KDY")]
        public DateTime ZmenenoKdy { get; set; }
        [DisplayName("ZMĚNĚNO KDO")]
        public string ZmenenoKdo { get; set; }
    }
}