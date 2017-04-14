using System;
using System.ComponentModel;

namespace MVC_WebService.Models
{
    public class ZnamkaLocalModel
    {
        public Guid Id { get; set; }
        [DisplayName("PŘEDMĚT")]
        public Guid Predmet { get; set; }
        public Guid Zak { get; set; }
        [DisplayName("ZNÁMKA")]
        public decimal Znamka { get; set; }

        public string PredmetNazev { get; set; }
    }
}