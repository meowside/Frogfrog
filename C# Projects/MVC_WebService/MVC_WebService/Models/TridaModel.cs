using System;
using System.ComponentModel;
using System.ComponentModel.DataAnnotations;

namespace MVC_WebService.Models
{
    public class TridaModel
    {
        public Guid Id { get; set; }

        [DisplayName("NÁZEV")]
        [Required]
        [StringLength(20, MinimumLength = 2)]
        public string Nazev { get; set; }

        [DisplayName("TŘÍDNÍ")]
        [Required]
        [StringLength(100, MinimumLength = 5)]
        public string Tridni { get; set; }

        [DisplayName("ZMĚNĚNO KDY")]
        [Required]
        [DataType(DataType.DateTime)]
        public DateTime ZmenenoKdy { get; set; }

        [DisplayName("ZMĚNENO KDO")]
        [Required]
        public string ZmenenoKdo { get; set; }

        [DisplayName("ROČNÍK")]
        [Range(1, 4)]
        [Required]
        public int? Rocnik { get; set; }

        [DisplayName("ULOŽENO")]
        public bool Ulozeno { get; set; }
    }
}