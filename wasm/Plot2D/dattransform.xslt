<?xml version="1.0"?>

<xsl:stylesheet version="1.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:template match="/">
  <html>
  <body>
    <form id="form42">
	<input type="button" value="Plot" onClick="parent.Module.ccall('doResume', 'null')"/>
    <table border="1">
      <tr bgcolor="#9acd32">
        <th>Name</th>
        <th>Input</th>
		<th></th>
        <th>Default</th>
        <th>Description</th>
      </tr>
      <xsl:for-each select="Dialog/Parameters/Parameter">
        <tr>
          <td><xsl:value-of select="@name"/></td>
          <td>
			<input id="{generate-id(.)}" type="text" name="{@name}" value="{@default}"/>
		  </td>
		  <td>
			<xsl:if test="@type = 'color'">
				<input id="picker{generate-id(.)}" type="{@type}" name="picker_{@name}" onchange="setColorValue('{generate-id(.)}', this.value);" />
			</xsl:if>			
			<xsl:if test="Value">
				<select>
					<xsl:for-each select="Value">
						<xsl:choose>
							<xsl:when test="'No' = 'No'">
								<option selected="selected" value="{current()}" onClick="document.getElementById('{generate-id(..)}').value='{current()}';"><xsl:value-of select="current()"/></option>
							</xsl:when>
							<xsl:otherwise>
								<option value="{current()}" onClick="document.getElementById('{generate-id(..)}').value='{current()}';"><xsl:value-of select="current()"/></option>
							</xsl:otherwise>
						</xsl:choose>
					</xsl:for-each>
				</select>
			</xsl:if>			
          </td>
          <td>
            <xsl:value-of select="@default"/>
          </td>          
          <td><xsl:value-of select="Description"/></td>
        </tr>
      </xsl:for-each>
    </table>
    <script>
        function getValueByName( parmname ) {
            var elements = document.getElementById("form42").elements ;
            for( var i = 0, element; element = elements[i++]; ) {
                if( element.name == parmname ) {
                    return element.value ;
                }
            }
            return "not found" ;
        }
		function dec( hexnumber ) {
			number = parseInt( hexnumber, 16 ) ;
			return number ;
		}		
		function setColorValue( id, colorhex ) {
			var r = colorhex.substring(1,3) ;
			var g = colorhex.substring(3,5) ;
			var b = colorhex.substring(5,7) ;			
			document.getElementById( id ).value = dec(r)+","+dec(g)+","+dec(b) ;
		}
		function getRGB(str,i)
		{
			var partsOfStr = str.split(',');
			var colorcomp = parseInt(partsOfStr[i]) ;
			return colorcomp ;
		}
		function hex( number ) {
			var hexString = number.toString(16);
			if (hexString.length % 2) {
				hexString = '0' + hexString;
			}
			return hexString ;
		}
        function parseParameters()
        {
            var elements = document.getElementById("form42").elements ;
            var params = '' ;
            for( var i = 0, element; element = elements[i++]; ) {
                params += (element.name + ": " + element.value) ;
				var picker = document.getElementById('picker' + element.id) ;
				if( picker != null ) {
					var myval = hex(getRGB(element.value,0)) + hex(getRGB(element.value,1)) + hex(getRGB(element.value,2));
					picker.value = "#"+myval.toString() ;
				}
				params += "\r\n" ;
            }
            //alert( params ) ;			
        }
		parseParameters() ;
    </script>
    <input type="button" value="Plot" onClick="parent.Module.ccall('doResume', 'null')"/>
	<input type="button" value="Searchroots" onClick="parent.Module.ccall('doResume1', 'null')"/>
	<input type="button" value="Eval" onClick="parent.Module.ccall('doResume2', 'null')"/>
    </form>
  </body>
  </html>
</xsl:template>

</xsl:stylesheet>